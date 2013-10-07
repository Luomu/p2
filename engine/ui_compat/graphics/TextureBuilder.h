#pragma once
#include "Libs.h"

namespace Graphics {
    class TextureBuilder {
    public:
        TextureBuilder(const std::string &filename);
        ~TextureBuilder();

	    static TextureBuilder UI(const std::string &filename) {
		    return TextureBuilder(filename);
	    }

        Texture *GetOrCreateTexture(Renderer *r, const std::string &type) {
            return 0;
        }

        const TextureDescriptor GetDescriptor() { return TextureDescriptor(); }
    };
}