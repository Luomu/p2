#pragma once
#include "RefCounted.h"

namespace Graphics {
    class TextureDescriptor {
    public:
        TextureDescriptor()
        : dataSize(0.f)
        , texSize(0.f)
        , generateMipmaps(false)
        , allowCompression(false)
        , numberOfMipMaps(0)
        { }
	    //const TextureFormat format;
	    const vector2f dataSize;
	    const vector2f texSize;
	    //const TextureSampleMode sampleMode;
	    const bool generateMipmaps;
	    const bool allowCompression;
	    const unsigned int numberOfMipMaps;
    };

	class Texture : public RefCounted {
    public:
        const TextureDescriptor &GetDescriptor() const { return m_descriptor; }

    private:
	    TextureDescriptor m_descriptor;
	};
}
