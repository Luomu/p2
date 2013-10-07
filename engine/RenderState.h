#pragma once
#include "Common.h"
/**
 * Render states are set as one block
 *  - state block is created and applied through device
 *  - contains depth, blend, rasterizer states
 *  - states are owned by video device, and shared
 */
namespace Video {
	struct RenderStateDesc {
		RenderStateDesc();

		BLEND_MODE blendMode;
		FILL_MODE  fillMode;
		CULL_MODE  cullMode;
		bool       depthWriteEnabled;
		bool       depthTestEnabled;
	};

	class RenderState {
	public:
		RenderState(const RenderStateDesc&);
		virtual ~RenderState();

		const RenderStateDesc &GetDesc() const { return m_desc; }

	protected:
		RenderStateDesc m_desc;
	};
}