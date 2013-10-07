#include "engine/gl/GLTexture.h"
#include "engine/OS.h"

using namespace Video;

namespace {
	GLenum mapInternalFormat(TEXTURE_FORMAT format) {
		switch (format) {
			case TEXTURE_RGB_888: return GL_RGB;
			case TEXTURE_RGBA_8888: return GL_RGBA;
			case TEXTURE_SRGB_888: return GL_SRGB8;
			case TEXTURE_SRGBA_8888: return GL_SRGB8_ALPHA8;
			case TEXTURE_DEPTH_24: return GL_DEPTH_COMPONENT;
			default: assert(0); return 0;
		}
	}

	GLenum mapImageFormat(TEXTURE_FORMAT format) {
		switch (format) {
		case TEXTURE_RGB_888:
		case TEXTURE_SRGB_888:
			return GL_RGB;
		case TEXTURE_RGBA_8888:
		case TEXTURE_SRGBA_8888:
			return GL_RGBA;
		case TEXTURE_DEPTH_24: return GL_DEPTH_COMPONENT;
		default: assert(0); return 0;
		}
	}

	GLenum mapWrapMode(TEXTURE_ADDRESS_MODE mode) {
		switch (mode) {
		case TEXTURE_ADDRESS_WRAP:
			return GL_REPEAT;
		default:
			return GL_CLAMP_TO_EDGE;
		}
	}
}

GLTexture::GLTexture(GLuint glTex, const TextureDesc &desc)
: m_handle(glTex)
{
	m_desc = desc;
	if (desc.depth > 0)
		m_target = GL_TEXTURE_3D;
	else if (desc.height > 0)
		m_target = GL_TEXTURE_2D;
	else
		m_target = GL_TEXTURE_1D;

	SDL_assert(desc.width > 0);
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &m_handle);
}

void GLTexture::Bind(U8 unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(m_target, m_handle);
}

void GLTexture::Unbind(U8 unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(m_target, 0);
}

void GLTexture::SetSamplerState(const SamplerStateDesc &sdesc)
{
	glBindTexture(m_target, m_handle);
	if (sdesc.filter == TEXTURE_FILTER_NEAREST) {
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	} else if (sdesc.filter == TEXTURE_FILTER_BILINEAR) {
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (m_desc.levels != 1)
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	} else if (sdesc.filter == TEXTURE_FILTER_TRILINEAR) {
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (m_desc.levels != 1)
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, mapWrapMode(sdesc.addressU));
	glTexParameteri(m_target, GL_TEXTURE_WRAP_T, mapWrapMode(sdesc.addressV));
	glTexParameteri(m_target, GL_TEXTURE_WRAP_R, mapWrapMode(sdesc.addressW));
}

GLTexture *GLTexture::FromDesc(const TextureDesc &desc)
{
	GLuint texo;
	glGenTextures(1, &texo);
	glBindTexture(GL_TEXTURE_2D, texo);
	glTexImage2D(GL_TEXTURE_2D, 0, mapInternalFormat(desc.format),
		desc.width, desc.height, 0, mapImageFormat(desc.format), GL_UNSIGNED_BYTE, 0);
	if (desc.levels != 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	}
	return new GLTexture(texo, desc);
}

GLTexture *GLTexture::FromFile(const std::string &filename)
{
	RefCountedPtr<FileSystem::FileData> filedata = FileSystem::gameDataFiles.ReadFile(filename);
	if (!filedata) {
		OS::Log("Could not load %s", filename.c_str());
		return nullptr;
	}

	SDL_RWops *datastream = SDL_RWFromConstMem(filedata->GetData(), filedata->GetSize());
	SDL_Surface *surface = IMG_Load_RW(datastream, 1);
	if (!surface)
		OS::Error("LoadSurfaceFromFile: %s\n", IMG_GetError());

	GLenum intFormat = GL_RGB;
	GLenum mode = GL_RGB;
	if (surface->format->BytesPerPixel == 4) {
		intFormat = GL_RGBA;
		mode = GL_RGBA;
	}

	GLuint texo;
	glGenTextures(1, &texo);
	glBindTexture(GL_TEXTURE_2D, texo);
	glTexImage2D(GL_TEXTURE_2D, 0, intFormat, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(surface);

	//fill out desc after-the-fact
	TextureDesc desc;
	desc.width = surface->w;
	desc.height = surface->h;
	desc.format = (mode == GL_RGB) ? TEXTURE_RGB_888 : TEXTURE_RGBA_8888;

	GLTexture *tex = new GLTexture(texo, desc);

	SamplerStateDesc sdesc;
	sdesc.filter = TEXTURE_FILTER_TRILINEAR;
	tex->SetSamplerState(sdesc);

	return tex;
}