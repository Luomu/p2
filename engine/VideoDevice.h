#pragma once
/**
 * Video device is responsible for
 *  - creating graphical resources:
 *  - creating and applying render states
 *  - applying shaders, textures, render targets
 *  - set up viewport, scissor
 *  - buffer swap
 */
#include "Common.h"
#include "RenderState.h"
#include "RenderTarget.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

namespace Video {
	class Device {
	public:
		Device();
		virtual ~Device();

		virtual void BeginFrame() { }
		virtual void EndFrame()   { }
		///swap buffers and present rendered result to screen
		virtual void Present() = 0;

		//-- resource creation
		// these will be pure virtual
		virtual VertexBuffer *CreateVertexBuffer(const VertexBufferDesc&)        = 0;
		virtual IndexBuffer  *CreateIndexBuffer(U16 numIndices, BUFFER_USAGE)    = 0;
		///return a render state block, shared and freed automatically
		virtual RenderState  *CreateRenderState(const RenderStateDesc&)          = 0;
		virtual RenderTarget *CreateRenderTarget(const RenderTargetDesc&)        = 0;
		///create an empty texture
		virtual Texture      *CreateTexture(const TextureDesc&)                  = 0;
		///create a texture from file
		virtual Texture      *CreateTexture(const std::string&)                  = 0;
		///create shader by name, optionally pass an array of shader macros, freed automatically
		virtual Shader       *CreateShader(const std::string&, const std::vector<ShaderMacro>* = 0) = 0;

		//--resource activation
		///pass zero to unbind (except for state)
		virtual void SetRenderState(RenderState*)   { }
		virtual void SetRenderTarget(RenderTarget*) { }
		virtual void SetShader(Shader*)             { }
		virtual void SetTexture(U8 unit, Texture*)  { }
		virtual void SetVertexBuffer(VertexBuffer*) { }
		virtual void SetIndexBuffer(IndexBuffer*)   { }

		//-- color/depth buffer operations
		virtual void ClearColor(Color c = Color::BLACK)      = 0;
		virtual void ClearDepth(float depth = 1.f)           = 0;
		virtual void SetViewport(U32 x, U32 y, U32 w, U32 h) = 0;
		///scissor test is always enabled
		virtual void SetScissor(U32 x, U32 y, U32 w, U32 h)  = 0;

		//-- draw operations
		///draw unindexed primitives
		virtual void Draw(U32 startVtx, U32 vtxCount) = 0;
		///draw primitives using the currently bound index buffer
		virtual void DrawIndexed(U32 idxCount)        = 0;

		//--some optional dev functions
		virtual void ReloadShaders() { }
	protected:
	};
}