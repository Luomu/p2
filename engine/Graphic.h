#pragma once
/*
 * A graphic is a drawable thing.
 * It implements a Draw method.
 */

#include "Common.h"

namespace Video {
	class Device;
}

class Graphic {
public:
	virtual ~Graphic();
	virtual void Draw(Video::Device*) { }
	virtual void SetOrient(const mat3 &o) { m_orient = o;    }
	virtual void SetPos(vec3 p)           { m_pos = p;       }

protected:
	vec3 m_pos;
	mat3 m_orient;
};