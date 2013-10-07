#ifndef _MATRIX4X4_H
#define _MATRIX4X4_H

#include "Libs.h"

namespace UI {
	namespace matrix4x4f {
		static glm::mat4 Identity() { return glm::mat4(1.f); }
        static glm::mat4 Translation(float x, float y, float z) { return glm::translate(glm::mat4(1.f), glm::vec3(x, y, z)); }
	}
}

#endif