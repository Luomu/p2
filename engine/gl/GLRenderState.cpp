#include "engine/gl/GLRenderState.h"

using namespace Video;

GLRenderState::GLRenderState(const RenderStateDesc &rsd)
: RenderState(rsd)
{
}

void GLRenderState::Activate()
{
	//blend mode
	switch (m_desc.blendMode) {
		case BLEND_NONE:
			glDisable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ZERO);
			break;
		case BLEND_ADDITIVE:
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		case BLEND_ALPHA:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BLEND_ALPHA_PREMULT:
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			break;
	}

	glDepthMask(m_desc.depthWriteEnabled ? GL_TRUE : GL_FALSE);

	if (m_desc.depthTestEnabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	switch (m_desc.fillMode) {
		case FILL_SOLID:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case FILL_WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
	}

	switch (m_desc.cullMode) {
		case CULL_NONE:
			glDisable(GL_CULL_FACE);
			break;
		case CULL_FRONT:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_CW);
			break;
		case CULL_BACK:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_CCW);
			break;
	}
}