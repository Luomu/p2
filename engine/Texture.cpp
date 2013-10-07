#include "Texture.h"

using namespace Video;

TextureDesc::TextureDesc()
: format(TEXTURE_NONE)
, width(0)
, height(0)
, depth(0)
, levels(0)
{
}

SamplerStateDesc::SamplerStateDesc()
: filter(TEXTURE_FILTER_BILINEAR)
, addressU(TEXTURE_ADDRESS_CLAMP)
, addressV(TEXTURE_ADDRESS_CLAMP)
, addressW(TEXTURE_ADDRESS_CLAMP)
{
}