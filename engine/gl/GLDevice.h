#pragma once
#include "engine/VideoDevice.h"

namespace Video {
	class GLDevice : public Device {
	public:
		//not final
		GLDevice(SDL_Window*, bool debug);
		virtual ~GLDevice();
		virtual void Present();

		///resource creation
		virtual VertexBuffer *CreateVertexBuffer(const VertexBufferDesc&);
		virtual IndexBuffer  *CreateIndexBuffer(U16 numIndices, BUFFER_USAGE);
		virtual RenderState  *CreateRenderState(const RenderStateDesc&);
		virtual RenderTarget *CreateRenderTarget(const RenderTargetDesc&);
		virtual Texture      *CreateTexture(const TextureDesc&);
		virtual Texture      *CreateTexture(const std::string&);
		virtual Shader       *CreateShader(const std::string&, const std::vector<ShaderMacro>*);

		///resource activation
		virtual void SetRenderState(RenderState*);
		virtual void SetRenderTarget(RenderTarget*);
		virtual void SetShader(Shader*);
		virtual void SetTexture(U8 unit, Texture*);
		virtual void SetVertexBuffer(VertexBuffer*);
		virtual void SetIndexBuffer(IndexBuffer*);

		///buffer operations
		virtual void ClearColor(Color c = Color::BLACK);
		virtual void ClearDepth(float depth = 0.f);
		virtual void SetViewport(U32 x, U32 y, U32 w, U32 h);
		virtual void SetScissor(U32 x, U32 y, U32 w, U32 h);

		///drawing
		virtual void Draw(U32 startVtx, U32 vtxCount);
		virtual void DrawIndexed(U32 idxCount);

		///misc functions
		virtual void ReloadShaders();
	private:
		SDL_Window   *m_window;
		SDL_GLContext m_ctx;
		std::unordered_map<U32, RenderState*> m_renderStates;
		std::unordered_map<std::string, Shader*> m_shaders;
	};
}