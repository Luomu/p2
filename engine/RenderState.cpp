#include "RenderState.h"

using namespace Video;

RenderStateDesc::RenderStateDesc()
: blendMode(BLEND_NONE)
, fillMode(FILL_SOLID)
, cullMode(CULL_BACK)
, depthTestEnabled(true)
, depthWriteEnabled(true)
{
}

RenderState::RenderState(const RenderStateDesc& d)
: m_desc(d)
{
}

RenderState::~RenderState()
{
}