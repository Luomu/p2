#pragma once
#include "Libs.h"
#include "Color.h"
#include "FontDescriptor.h"

namespace Graphics { class Renderer; }

namespace Text {
	class TextureFont : public RefCounted {
    public:
        TextureFont(const FontDescriptor &descriptor, Graphics::Renderer *renderer) { }
        void RenderString(const char *str, float x, float y, const Color &color = Color::WHITE) { }
        void MeasureString(const char *str, float &w, float &h) { w = 0.f; h = 0.f; }
        void MeasureCharacterPos(const char *str, int charIndex, float &x, float &y) const { x = 0.f; y = 0.f; }
	    struct glfglyph_t {
		    float advx, advy;
		    float width, height;
		    float texWidth, texHeight;
		    int offx, offy;
		    float offU, offV; //atlas UV offset
		    Uint32 ftIndex;
	    };
	    const glfglyph_t GetGlyph(Uint32 ch) const { return glfglyph_t(); }
        float GetHeight() const { return 0.f; }
	};
}
