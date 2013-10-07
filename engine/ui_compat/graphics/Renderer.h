#pragma once
/*
 * UI renderer wrapper
 */
#include "engine/Common.h"
#include "Material.h"
#include "VertexArray.h"

namespace Graphics {
	enum BlendMode {
		BLEND_ALPHA,
		BLEND_SET_ALPHA,
		BLEND_DEST_ALPHA
	};

    enum LineType {
	    LINE_SINGLE,
	    LINE_STRIP,
	    LINE_LOOP
    };

    enum PrimitiveType {
	    TRIANGLES,
	    TRIANGLE_STRIP,
	    TRIANGLE_FAN,
	    POINTS
    };

	class Renderer {
    public:
        virtual bool SetDepthTest(bool) { return false; }
        virtual bool SetClearColor(const Color &c) { return false; }
        virtual bool SetBlendMode(BlendMode type) { return false; }
        virtual bool SetTransform(const mat4 &m) { return false; }
        virtual bool SetOrthographicProjection(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax) { return false; }
        virtual bool SetScissor(bool enabled, const vector2f &pos = vector2f(0.0f), const vector2f &size = vector2f(0.0f)) { return false; }

        virtual bool DrawLines(int vertCount, const vector3f *vertices, const Color &color, LineType type=LINE_SINGLE) { return false; }
        virtual bool DrawTriangles(const VertexArray *vertices, Material *material, PrimitiveType type=TRIANGLES)  { return false; }

        virtual Material *CreateMaterial(const MaterialDescriptor &descriptor) { return 0; }
	};
}
