// Adapted from (I wrote it anyway) Pioneer
//  - added MRT support
// Copyright © 2008-2013 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#pragma once
/*
 * Render target. Created by filling out a description and calling
 * device->CreateRenderTarget.
 */
#include "Texture.h"

namespace Video {
const U8 MAX_MRT = 4;

// A render target may have multiple color textures, depth buffer/texture or both.
// Setting the formats to NONE will skip the texture creation, and you will
// have to set the textures yourself.
// Only request allowDepthTexture if you actually need to read the depth as texture
// Specifying a depth format with no allowDepthTexture will create a depth buffer
// fixed to this rendertarget
struct RenderTargetDesc {
	U16 width;
	U16 height;
	TEXTURE_FORMAT colorFormat[MAX_MRT];
	TEXTURE_FORMAT depthFormat;
	bool allowDepthTexture;
};

class RenderTarget : public VideoResource {
public:
	virtual ~RenderTarget() { }

	virtual Texture *GetColorTexture(U8 index) const = 0;
	virtual Texture *GetDepthTexture() const = 0;
	//Replace the nth texture attachment, or pass zero to detach
	//Increases the new texture's reference count and decreases
	//any existing texture's count
	//Setting a depth texture is not allowed if the render target is not
	//created with allowDepthTexture
	virtual void SetColorTexture(U8 index, Texture*) = 0;
	virtual void SetDepthTexture(Texture*) = 0;

	const RenderTargetDesc &GetDesc() const { return m_desc; }

protected:
	RenderTarget(const RenderTargetDesc &d) : m_desc(d) { }

	RenderTargetDesc m_desc;
};
}