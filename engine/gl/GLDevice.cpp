#include "engine/gl/GLDevice.h"
#include "engine/gl/GlDebug.h"
#include "engine/gl/GLTexture.h"
#include "engine/gl/GLVertexBuffer.h"
#include "engine/gl/GLIndexBuffer.h"
#include "engine/gl/GLProgram.h"
#include "engine/gl/GLRenderState.h"
#include "engine/gl/GLRenderTarget.h"
#include "engine/OS.h"
#include <jenkins/lookup3.h>

using namespace Video;

namespace State {
	RenderTarget *renderTarget = nullptr;
}

GLDevice::GLDevice(SDL_Window* win, bool debug)
: m_window(win)
, m_ctx(nullptr)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	m_ctx = SDL_GL_CreateContext(m_window);
	if (m_ctx == nullptr) {
		printf("%s\n", SDL_GetError());
		throw std::string("GLContext init failure");
	}

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		printf("ogl_LoadFunctions failed\n");
		throw std::string("glLoader fail");
	}

	if (SDL_GL_SetSwapInterval(1) == -1)
		printf("SetSwapInterval: %s\n", SDL_GetError());

	printf("Version %s\n",  glGetString(GL_VERSION));
	printf("Vendor %s\n",   glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));

	if (debug)
		GlDebug::Init();
}

GLDevice::~GLDevice()
{
	OS::Log("Deleting %u shaders", m_shaders.size());
	for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
		delete it->second;
	OS::Log("Deleting %u render states", m_renderStates.size());
	for (auto it = m_renderStates.begin(); it != m_renderStates.end(); ++it)
		delete it->second;
	SDL_GL_DeleteContext(m_ctx);
}

void GLDevice::Present()
{
	SDL_GL_SwapWindow(m_window);
}

/// resource creation

VertexBuffer *GLDevice::CreateVertexBuffer(const VertexBufferDesc &desc)
{
	SDL_assert(desc.numVertices > 0);
	return new GLVertexBuffer(desc);
}

IndexBuffer *GLDevice::CreateIndexBuffer(U16 numIndices, BUFFER_USAGE usage)
{
	SDL_assert(numIndices > 0);
	return new GLIndexBuffer(numIndices, usage);
}

RenderState *GLDevice::CreateRenderState(const RenderStateDesc &desc)
{
	U32 hash = lookup3_hashlittle(&desc, sizeof(RenderStateDesc), 0);
	auto it = m_renderStates.find(hash);
	if (it != m_renderStates.end())
		return it->second;
	GLRenderState *s = new GLRenderState(desc);
	m_renderStates[hash] = s;
	return s;
}

RenderTarget *GLDevice::CreateRenderTarget(const RenderTargetDesc &desc)
{
	return GLRenderTarget::FromDesc(desc);
}

Texture *GLDevice::CreateTexture(const TextureDesc &desc)
{
	return GLTexture::FromDesc(desc);
}

Texture *GLDevice::CreateTexture(const std::string &name)
{
	return GLTexture::FromFile(name);
}

Shader *GLDevice::CreateShader(const std::string &name, const std::vector<ShaderMacro> *macros)
{
	//should look store macroed shaders separately
	auto it = m_shaders.find(name);
	if (it != m_shaders.end())
		return it->second;
	GLProgram *prog = new GLProgram(name, macros);
	m_shaders[name] = prog;
	return prog;
}

/// resource activation

void GLDevice::SetRenderState(RenderState *state)
{
	static_cast<GLRenderState*>(state)->Activate();
}

void GLDevice::SetRenderTarget(RenderTarget *tgt)
{
	if (tgt)
		static_cast<GLRenderTarget*>(tgt)->Bind();
	else if (State::renderTarget)
		static_cast<GLRenderTarget*>(State::renderTarget)->Unbind();

	State::renderTarget = tgt;
}

void GLDevice::SetShader(Shader *shader)
{
	if (shader)
		static_cast<GLProgram*>(shader)->Use();
	else
		glUseProgram(0);
}

void GLDevice::SetTexture(U8 unit, Texture *texture)
{
	if (texture)
		static_cast<GLTexture*>(texture)->Bind(unit);
	else {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void GLDevice::SetVertexBuffer(VertexBuffer *buf)
{
	if (buf)
		static_cast<GLVertexBuffer*>(buf)->PrepareForDraw();
	else
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLDevice::SetIndexBuffer(IndexBuffer *buf)
{
	if (buf)
		static_cast<GLIndexBuffer*>(buf)->Bind();
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/// buffer operations

void GLDevice::ClearColor(Color c)
{
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLDevice::ClearDepth(float depth)
{
	glClearDepthf(depth);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GLDevice::SetViewport(U32 x, U32 y, U32 w, U32 h)
{
	glViewport(x, y, w, h);
}

void GLDevice::SetScissor(U32 x, U32 y, U32 w, U32 h)
{
	glScissor(x, y, w, h);
}

/// drawing

void GLDevice::Draw(U32 startVtx, U32 vtxCount)
{
	glDrawArrays(GL_TRIANGLES, startVtx, vtxCount);
}

void GLDevice::DrawIndexed(U32 idxCount)
{
	glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_SHORT, 0);
}

/// reloading

void GLDevice::ReloadShaders()
{
	for (auto it = m_shaders.begin(); it != m_shaders.end(); ++it)
		it->second->Reload();
}