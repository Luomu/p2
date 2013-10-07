#include "engine/gl/GLRenderTarget.h"
#include "engine/gl/GLTexture.h"

using namespace Video;

GLRenderBuffer::GLRenderBuffer()
{
	glGenRenderbuffers(1, &buffer);
}

GLRenderBuffer::~GLRenderBuffer()
{
	glDeleteRenderbuffers(1, &buffer);
}

void GLRenderBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, buffer);
}

void GLRenderBuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GLRenderBuffer::Attach(GLenum attachment)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer);
}

GLRenderTarget::GLRenderTarget(const RenderTargetDesc &d)
: RenderTarget(d)
, m_active(false)
, m_numBuffers(0)
{
	glGenFramebuffers(1, &m_fbo);

	for (U8 i = 0; i < MAX_MRT; i++) {
		if (d.colorFormat[i] == TEXTURE_NONE) break;
		m_numBuffers++;
		m_drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
}

GLRenderTarget::~GLRenderTarget()
{
	glDeleteFramebuffers(1, &m_fbo);
}

Texture *GLRenderTarget::GetColorTexture(U8 idx) const
{
	SDL_assert(idx < MAX_MRT);
	return m_colorTexture[idx].Valid() ? m_colorTexture[idx].Get() : 0;
}

Texture *GLRenderTarget::GetDepthTexture() const
{
	assert(GetDesc().allowDepthTexture);
	return m_depthTexture.Valid() ? m_depthTexture.Get() : 0;
}

void GLRenderTarget::SetColorTexture(U8 idx, Texture* t)
{
	SDL_assert(idx < MAX_MRT);
	const bool bound = m_active;
	if (!bound) Bind();
	//texture format should match the intended fbo format (aka. the one attached first)
	GLuint texId = 0;
	if (t) texId = static_cast<GLTexture*>(t)->GetHandle();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, texId, 0);
	m_colorTexture[idx].Reset(t);
	if (!bound) Unbind();
}

void GLRenderTarget::SetDepthTexture(Texture* t)
{
	SDL_assert(GetDesc().allowDepthTexture);
	if (!GetDesc().allowDepthTexture) return;
	const bool bound = m_active;
	if (!bound) Bind();
	GLuint texId = 0;
	if (t) texId = static_cast<GLTexture*>(t)->GetHandle();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texId, 0);
	m_depthTexture.Reset(t);
	if (!bound) Unbind();
}

void GLRenderTarget::Bind()
{
	SDL_assert(!m_active);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glDrawBuffers(m_numBuffers, m_drawBuffers);
	m_active = true;
}

void GLRenderTarget::Unbind()
{
	assert(m_active);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_active = false;
}

bool GLRenderTarget::CheckStatus()
{
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void GLRenderTarget::CreateDepthRenderbuffer()
{
	assert(!GetDesc().allowDepthTexture);
	assert(m_active);
	m_depthRenderBuffer.Reset(new GLRenderBuffer());
	m_depthRenderBuffer->Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, GetDesc().width, GetDesc().height);
	m_depthRenderBuffer->Attach(GL_DEPTH_ATTACHMENT);
	m_depthRenderBuffer->Unbind();
}

GLRenderTarget *GLRenderTarget::FromDesc(const RenderTargetDesc &desc)
{
	GLRenderTarget *target = new GLRenderTarget(desc);

	TextureDesc tdesc;
	tdesc.width  = desc.width;
	tdesc.height = desc.height;
	tdesc.levels = 1;

	target->Bind();
	for (U8 i = 0; i < MAX_MRT; i++) {
		if (desc.colorFormat[i] == TEXTURE_NONE) break;
		tdesc.format = desc.colorFormat[i];
		GLTexture* colorTex = GLTexture::FromDesc(tdesc);
		target->SetColorTexture(i, colorTex);
	}
	if (desc.depthFormat == TEXTURE_DEPTH_24) {
		if (desc.allowDepthTexture) {
			tdesc.format = desc.depthFormat;
			GLTexture* depthTex = GLTexture::FromDesc(tdesc);
			target->SetDepthTexture(depthTex);
		} else {
			target->CreateDepthRenderbuffer();
		}
	}
	target->Unbind();

	return target;
}