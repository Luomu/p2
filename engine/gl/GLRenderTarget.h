#pragma once
/*
 * Framebuffer object with switchable target textures.
 * In theory you should use one texture format and size per FBO
 * 2013-May-05 left out stencil buffer because we don't need it now
 */
#include "engine/RenderTarget.h"

namespace Video {
	class GLRenderBuffer : public RefCounted {
	public:
		void Bind();
		void Unbind();
		void Attach(GLenum attachment);

	protected:
		friend class GLRenderTarget;
		GLRenderBuffer();
		~GLRenderBuffer();
		GLuint buffer;
	};

	class GLRenderTarget : public RenderTarget {
	public:
		~GLRenderTarget();
		virtual Texture *GetColorTexture(U8) const;
		virtual Texture *GetDepthTexture() const;
		virtual void SetColorTexture(U8, Texture*);
		virtual void SetDepthTexture(Texture*);

		//create target and textures
		static GLRenderTarget *FromDesc(const RenderTargetDesc&);

		void Bind();
		void Unbind();

	protected:
		//does not create attachments, just the FBO
		GLRenderTarget(const RenderTargetDesc &);
		void CreateDepthRenderbuffer();
		bool CheckStatus();

		bool   m_active;
		GLuint m_fbo;

		RefCountedPtr<GLRenderBuffer> m_depthRenderBuffer;
		TexturePtr m_colorTexture[MAX_MRT];
		TexturePtr m_depthTexture;

		//some cached values for setting up draw
		U8     m_numBuffers;
		GLuint m_drawBuffers[MAX_MRT];
	};
}
