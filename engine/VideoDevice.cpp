#include "VideoDevice.h"

using namespace Video;

Device::Device()
{
	m_worldMatrix = mat4(1.f);
	m_viewMatrix  = mat4(1.f);
	m_projMatrix  = mat4(1.f);

	m_viewport = ivec4(0, 0, 640, 480);
}

Device::~Device()
{
}

void Device::SetWorldMatrix(const mat4 &m)
{
	m_worldMatrix = m;
}

void Device::SetViewMatrix(const mat4 &m)
{
	m_viewMatrix = m;
}

void Device::SetProjectionMatrix(const mat4 &m)
{
	m_projMatrix = m;
}