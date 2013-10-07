#include "VertexBuffer.h"

using namespace Video;

U32 VertexBufferDesc::AttribSize(ATTRIB_FORMAT f)
{
	switch (f) {
	case ATTRIB_FORMAT_FLOAT2:
		return 8;
	case ATTRIB_FORMAT_FLOAT3:
		return 12;
	case ATTRIB_FORMAT_FLOAT4:
		return 16;
	case ATTRIB_FORMAT_UBYTE4:
		return 4;
	default:
		return 0;
	}
}

U32 VertexBufferDesc::VertexSize() const
{
	U32 size = 0;
	for (U8 i = 0; i < MAX_ATTRIBS; i++) {
		if (attribs[i].semantic == ATTRIB_NONE) break;
		size += AttribSize(attribs[i].format);
	}
	return size;
}

U32 VertexBufferDesc::Offset(ATTRIB_SEMANTIC attrib) const
{
	U32 offset = 0;
	for (U8 i = 0; i < MAX_ATTRIBS; i++) {
		if (attribs[i].semantic == attrib) break;
		if (attribs[i].semantic == ATTRIB_NONE) break;
		offset += AttribSize(attribs[i].format);
	}
	return offset;
}

VertexBuffer::VertexBuffer(const VertexBufferDesc &desc)
: m_desc(desc)
{
}

VertexBuffer::~VertexBuffer()
{
}