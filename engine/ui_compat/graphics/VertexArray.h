#pragma once
#include "libs.h"

namespace Graphics {
    enum VertexAttrib {
	    ATTRIB_POSITION  = (1u << 0),
	    ATTRIB_NORMAL    = (1u << 1),
	    ATTRIB_DIFFUSE   = (1u << 2),
	    ATTRIB_UV0       = (1u << 3),
    };

    typedef unsigned int AttributeSet;

    class VertexArray {
    public:
        VertexArray(AttributeSet attribs, int size=0) { }

        virtual void Add(const vector3f &v) { }
        virtual void Add(const vector3f &v, const vector2f &uv) { }
        virtual void Add(const vector3f &v, const Color &uv) { }
    };
}