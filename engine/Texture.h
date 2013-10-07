#pragma once
#include "VideoResource.h"

namespace Video {
	struct TextureDesc {
		TextureDesc();

		TEXTURE_FORMAT format;

		U32 width;
		U32 height;
		U32 depth;

		///0 = generate full set
		U32 levels;
	};

	struct SamplerStateDesc {
		SamplerStateDesc();

		TEXTURE_FILTER_MODE  filter;
		TEXTURE_ADDRESS_MODE addressU;
		TEXTURE_ADDRESS_MODE addressV;
		TEXTURE_ADDRESS_MODE addressW;
	};

	class Texture : public VideoResource {
	public:
		Texture() { }
		virtual ~Texture() { }

		const TextureDesc &GetDesc() const { return m_desc; }

		virtual void SetSamplerState(const SamplerStateDesc&) = 0;

	protected:
		TextureDesc m_desc;
	};

	typedef RefCountedPtr<Texture> TexturePtr;
}