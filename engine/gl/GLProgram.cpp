// Shader loading & compiling code adapted from Pioneer:
// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#include "GLProgram.h"
#include "contrib/pioneer/FileSystem.h"
#include "engine/OS.h"

const char *GLSL_VERSION      = "#version 430 core\n";
const std::string SHADER_PATH = "shaders/gl/";

using namespace Video;

// Check and warn about compile & link errors
namespace {
	bool check_glsl_errors(const char *filename, GLuint obj)
	{
		//check if shader or program
		bool isShader = (glIsShader(obj) == GL_TRUE);

		int infologLength = 0;
		char infoLog[1024];

		if (isShader)
			glGetShaderInfoLog(obj, 1024, &infologLength, infoLog);
		else
			glGetProgramInfoLog(obj, 1024, &infologLength, infoLog);

		GLint status;
		if (isShader)
			glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
		else
			glGetProgramiv(obj, GL_LINK_STATUS, &status);

		if (status == GL_FALSE) {
			OS::Log("Error compiling shader %s: %s", filename, infoLog);
			return false;
		}

		// Log warnings even if successfully compiled
		// Sometimes the log is full of junk "success" messages
		if (infologLength > 0)
			OS::Log("%s: %s", filename, infoLog);

		return true;
	}

	SHADER_CONST_TYPE FromGLType(GLenum t)
	{
		switch (t) {
		case GL_FLOAT:      return SHADER_CONST_FLOAT;
		case GL_FLOAT_VEC2: return SHADER_CONST_FLOAT2;
		case GL_FLOAT_VEC3: return SHADER_CONST_FLOAT3;
		case GL_FLOAT_VEC4: return SHADER_CONST_FLOAT4;
		case GL_FLOAT_MAT4: return SHADER_CONST_MAT4;
		default:            return SHADER_CONST_NONE;
		}
	}

	U32 SizeFromGLType(GLenum t)
	{
		switch (t) {
		case GL_FLOAT:      return 4;
		case GL_FLOAT_VEC2: return 8;
		case GL_FLOAT_VEC3: return 12;
		case GL_FLOAT_VEC4: return 16;
		case GL_FLOAT_MAT4: return 64;
		default:            return 0;
		}
	}

	const char *ToString(GLenum t)
	{
	#define CHECK(name) case(name): return #name;
		switch (t) {
			CHECK(GL_FLOAT)
			CHECK(GL_FLOAT_VEC2)
			CHECK(GL_FLOAT_VEC3)
			CHECK(GL_FLOAT_VEC4)
			CHECK(GL_INT)
			CHECK(GL_INT_VEC2)
			CHECK(GL_INT_VEC3)
			CHECK(GL_INT_VEC4)
			CHECK(GL_BOOL)
			CHECK(GL_BOOL_VEC2)
			CHECK(GL_BOOL_VEC3)
			CHECK(GL_BOOL_VEC4)
			CHECK(GL_FLOAT_MAT2)
			CHECK(GL_FLOAT_MAT3)
			CHECK(GL_FLOAT_MAT4)
			CHECK(GL_SAMPLER_2D)
			CHECK(GL_SAMPLER_CUBE)
			default: return "Unknown";
		}
	#undef CHECK
	}
}

// Mainly RAII wrapper for glCreate/DeleteShader
struct GLShader {
	GLShader(GLenum type, const std::string &filename, const std::string &defines)
	{
		RefCountedPtr<FileSystem::FileData> code = FileSystem::gameDataFiles.ReadFile(filename);

		if (!code)
			OS::Error("Could not load %s", filename.c_str());

		AppendSource(GLSL_VERSION);

		if (!defines.empty())
			AppendSource(defines.c_str());

		if (type == GL_VERTEX_SHADER)
			AppendSource("#define VERTEX_SHADER\n");
		else
			AppendSource("#define FRAGMENT_SHADER\n");

		//keep these until after glShaderSource
		std::vector<RefCountedPtr<FileSystem::FileData>> includeHandles;

		//handle includes
		StringRange csr = code->AsStringRange().StripUTF8BOM();
		const char *p = strstr(csr.begin, "#include");
		const char *blockBegin = csr.begin;
		while (p) {
			const char *incStart = p;
			p += 8;
			while (isspace(*p)) ++p;
			SDL_assert(*p == '"');
			++p;
			//64 char max filename
			static char includeFile[64];
			U32 i = 0;
			while (*p != '"') {
				includeFile[i++] = *p++;
				if (i >=64)
					OS::Error("GLSL include name longer than 64 chars");
			}
			++p;
			includeFile[i] = 0;

			//append what we have so far
			AppendSource(StringRange(blockBegin, incStart));
			blockBegin = p; //end of last include

			//Got a file! try to load it
			const std::string incFileName = FileSystem::JoinPathBelow(code->GetInfo().GetDir(), includeFile);
			RefCountedPtr<FileSystem::FileData> incCode = FileSystem::gameDataFiles.ReadFile(incFileName);
			includeHandles.push_back(incCode);

			//should handle includes recursively but nah
			AppendSource(incCode->AsStringRange().StripUTF8BOM());

			//keep looking for includes
			p = strstr(p, "#include");
		}
		AppendSource(StringRange(blockBegin, csr.end));

		shader = glCreateShader(type);
		Compile(shader);

		// CheckGLSL may use OS::Warning instead of OS::Error so the game may still (attempt to) run
		if (!check_glsl_errors(filename.c_str(), shader))
			throw GLProgram::ShaderException();
	};

	~GLShader()
	{
		glDeleteShader(shader);
	}

	GLuint shader;

private:
	void AppendSource(const char *str)
	{
		blocks.push_back(str);
		block_sizes.push_back(std::strlen(str));
	}

	void AppendSource(StringRange str)
	{
		blocks.push_back(str.begin);
		block_sizes.push_back(str.Size());
	}

	void Compile(GLuint shader_id)
	{
		assert(blocks.size() == block_sizes.size());
		glShaderSource(shader_id, blocks.size(), &blocks[0], &block_sizes[0]);
		glCompileShader(shader_id);
	}

	std::vector<const char*> blocks;
	std::vector<GLint> block_sizes;
};

GLConstHandle::GLConstHandle(const std::string &name)
: ShaderConstHandle(name)
, m_location(-1)
{
}

void GLConstHandle::Reset(SHADER_CONST_TYPE type, GLint location, U32 offset)
{
	m_type     = type;
	m_location = location;
	m_offset   = offset;
	m_valid    = true;
}

GLConstBuffer::GLConstBuffer(size_t bytes)
: ShaderConstBuffer(bytes)
{
}

GLProgram::GLProgram()
: m_program(0)
, m_constBufferSize(0)
{
}

GLProgram::GLProgram(const std::string &name, const std::vector<ShaderMacro> *macros)
: m_program(0)
, m_constBufferSize(0)
{
	m_name = name;
	if (macros) {
		for (auto it = macros->begin(); it != macros->end(); ++it)
			m_macros.push_back(*it);
	}

	TryLoad();
}

GLProgram::~GLProgram()
{
	for (auto it = m_handles.begin(); it != m_handles.end(); ++it)
		delete it->second;
	glDeleteProgram(m_program);
}

void GLProgram::Reload()
{
	Unuse();
	glDeleteProgram(m_program);
	TryLoad();
}

void GLProgram::Use()
{
	glUseProgram(m_program);
}

void GLProgram::Unuse()
{
	glUseProgram(0);
}

ShaderConstBuffer *GLProgram::CreateConstBuffer()
{
	SDL_assert(m_constBufferSize > 0);
	return new GLConstBuffer(m_constBufferSize);
}

ShaderConstHandle *GLProgram::GetConstHandle(const std::string &name)
{
	auto it = m_handles.find(name);
	if (it != m_handles.end())
		return it->second;
	else {
		//return an invalid handle
		GLConstHandle *h = new GLConstHandle(name);
		m_handles[name] = h;
		return h;
	}
}

void GLProgram::ApplyConstants(ShaderConstBuffer *buf)
{
	for (auto it = m_handles.begin(); it != m_handles.end(); ++it) {
		if (!it->second->IsValid()) continue;
		GLConstHandle *ch = static_cast<GLConstHandle*>(it->second);
		switch (ch->GetType())
		{
		case SHADER_CONST_FLOAT:
			glUniform1fv(ch->GetLocation(), 1, reinterpret_cast<const GLfloat*>(buf->GetData() + ch->GetOffset()));
			break;
		case SHADER_CONST_FLOAT2:
			glUniform2fv(ch->GetLocation(), 1, reinterpret_cast<const GLfloat*>(buf->GetData() + ch->GetOffset()));
			break;
		case SHADER_CONST_FLOAT3:
			glUniform3fv(ch->GetLocation(), 1, reinterpret_cast<const GLfloat*>(buf->GetData() + ch->GetOffset()));
			break;
		case SHADER_CONST_FLOAT4:
			glUniform4fv(ch->GetLocation(), 1, reinterpret_cast<const GLfloat*>(buf->GetData() + ch->GetOffset()));
			break;
		case SHADER_CONST_MAT4:
			glUniformMatrix4fv(ch->GetLocation(), 1, GL_FALSE, reinterpret_cast<const GLfloat*>(buf->GetData() + ch->GetOffset()));
			break;
		default:
			break;
		}
	}
}

void GLProgram::LoadShaders(const std::string &name, const std::string &defines)
{
	const std::string filename = SHADER_PATH + name;

	//load, create and compile shaders
	GLShader vs(GL_VERTEX_SHADER, filename + ".glsl", defines);
	GLShader fs(GL_FRAGMENT_SHADER, filename + ".glsl", defines);

	//create program, attach shaders and link
	m_program = glCreateProgram();
	glAttachShader(m_program, vs.shader);
	glAttachShader(m_program, fs.shader);
	glLinkProgram(m_program);

	check_glsl_errors(name.c_str(), m_program);

	InitUniforms();
}

void GLProgram::InitUniforms()
{
	//mark all handles invalid
	for (auto it = m_handles.begin(); it != m_handles.end(); ++it)
		it->second->SetValid(false);

	GLint numUniforms, maxNameLen;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLen);
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &numUniforms);

	GLchar *name = new GLchar[maxNameLen];

	GLint size, location;
	GLsizei written;
	GLenum type;

	U32 offset = 0;
	GLuint samplerIdx = 0;

	for (int i = 0; i < numUniforms; i++) {
		glGetActiveUniform(m_program, i, maxNameLen, &written, &size, &type, name);
		location = glGetUniformLocation(m_program, name);
		OS::Log("loc %d | %s | size %d | type %s", location, name, size, ToString(type));

		//sampler uniforms are not added as handles, but set immediately
		if (type == GL_SAMPLER_2D) {
			glUniform1i(location, samplerIdx);
			samplerIdx++;
			continue;
		}
		//filter out unsupported types for now
		SHADER_CONST_TYPE constType = FromGLType(type);
		if (type == SHADER_CONST_NONE) {
			OS::Log("(unsupported, skipping)");
			continue;
		}

		//create new handle or update existing
		GLConstHandle* ch = static_cast<GLConstHandle*>(GetConstHandle(name));
		ch->Reset(constType, location, offset);

		offset += SizeFromGLType(type);
	}

	m_constBufferSize = offset;

	delete[] name;
}

void GLProgram::TryLoad()
{
	SDL_assert(!m_name.empty());

	try {
		//stringize here so we don't pass possible erroneus macros to
		//fallback shader
		std::stringstream ss;
		for (auto it = m_macros.begin(); it != m_macros.end(); ++it)
			ss << "#define " << it->name << " " << it->value << std::endl;
		std::string defines = ss.str();
		LoadShaders(m_name, defines);
	} catch (GLProgram::ShaderException &) {
		OS::Warning("Error in %s", m_name.c_str());
		try {
			LoadShaders("test", "");
		} catch (...) {
			OS::Error("Could not load fallback shader");
		}
	}
}