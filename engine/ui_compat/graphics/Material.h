#pragma once
#include "RefCounted.h"
#include "Color.h"
#include "Texture.h"

namespace Graphics {
    class MaterialDescriptor {
    public:
	    //MaterialDescriptor();
	    //EffectType effect;
	    //bool alphaTest;
	    //bool atmosphere;
	    //bool glowMap;
	    //bool lighting;
	    //bool specularMap;
	    //bool twoSided;
	    //bool usePatterns; //pattern/color system
	    bool vertexColors;
	    int textures; //texture count
	    //unsigned int dirLights; //set by rendererGL2 if lighting == true
    };

	class Material : public RefCounted {
	public:
		Color4f diffuse;
        Texture *texture0;
	};
}
