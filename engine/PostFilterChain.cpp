#include "PostFilterChain.h"

using namespace Video;

PostFilter::PostFilter()
: m_shader(nullptr)
, m_sources()
{
}

PostFilter::~PostFilter()
{
}

void PostFilterChain::Apply()
{
	for (auto it = m_filters.begin(); it != m_filters.end(); ++it)
		it->get()->Apply();
}

void PostFilterChain::Clear()
{
	m_filters.clear();
}