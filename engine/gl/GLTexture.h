#pragma once
#include "engine/Texture.h"

namespace Video {
	class GLTexture : public Texture {
	public:
		///create from already loaded texture (description must match)
		GLTexture(GLuint glTex, const TextureDesc&);
		~GLTexture();
		GLuint GetHandle() const { return m_handle; }

		void Bind(U8 unit);
		void Unbind(U8 unit);

		virtual void SetSamplerState(const SamplerStateDesc&);

		static GLTexture *FromDesc(const TextureDesc &);
		static GLTexture *FromFile(const std::string &);

	private:
		GLuint m_handle;
		GLenum m_target; ///figured out from description
	};
}