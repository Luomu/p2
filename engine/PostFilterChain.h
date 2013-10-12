#pragma once
/**
 * Post-processing filter & filter chain
 * A filter has source texture(s), shader
 * and renders to a rendertarget (or screen
 * in case of the last filter in the chain)
 *
 * Filters can and should share render targets,
 * as long as source and target are not the same.
 */
#include "Common.h"
#include "RenderTarget.h"
#include "Shader.h"

namespace Video {
	class PostFilter {
	public:
		PostFilter();
		virtual ~PostFilter();
		///sets up the filter parameters and draws to target
		virtual void Apply() = 0;

	protected:
		RefCountedPtr<Video::RenderTarget> m_target;
		Video::Texture *m_sources[4];
		Video::Shader  *m_shader;
		std::unique_ptr<Video::ShaderConstBuffer> m_cbuffer;
	};

	/**
	 * Filters are unique and owned by the chain.
	 * Note: you can only add new passes at the end
	 * or clear the entire chain.
	 */
	class PostFilterChain {
	public:
		void Apply();
		/// destroys all filters
		void Clear();

		std::vector<std::unique_ptr<PostFilter>> m_filters;
	};
};