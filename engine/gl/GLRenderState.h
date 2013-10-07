#pragma once
#include "engine/RenderState.h"

namespace Video {
	class GLRenderState : public RenderState {
	public:
		GLRenderState(const RenderStateDesc&);
		void Activate();
	};
}