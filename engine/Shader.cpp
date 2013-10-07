#include "engine/Shader.h"

using namespace Video;

ShaderMacro::ShaderMacro(const std::string &name_, const std::string &value_)
: name(name_), value(value_) { }

ShaderConstHandle::ShaderConstHandle(const std::string &name)
: m_name(name)
, m_type(SHADER_CONST_NONE)
, m_offset(0)
, m_valid(false)
{
}

ShaderConstBuffer::ShaderConstBuffer(size_t size)
{
	m_data = new U8[size];
}

ShaderConstBuffer::~ShaderConstBuffer()
{
	delete[] m_data;
}

template<typename T>
void ShaderConstBuffer::CopyData(ShaderConstHandle *handle, const T &value)
{
	if (handle->IsValid())
		memcpy(m_data + handle->GetOffset(), &value, sizeof(T));
}

void ShaderConstBuffer::Set(ShaderConstHandle *handle, vec2 value)
{
	CopyData(handle, value);
}

void ShaderConstBuffer::Set(ShaderConstHandle *handle, vec3 value)
{
	CopyData(handle, value);
}

void ShaderConstBuffer::Set(ShaderConstHandle *handle, vec4 value)
{
	CopyData(handle, value);
}

void ShaderConstBuffer::Set(ShaderConstHandle *handle, const mat4 &value)
{
	CopyData(handle, value);
}

Shader::~Shader()
{
}