#pragma once

#include "Common.h"

namespace Video {
	const U8 MAX_ATTRIBS = 8;

	struct VertexDesc {
		ATTRIB_SEMANTIC semantic;
		ATTRIB_FORMAT   format;
	};

	struct VertexBufferDesc {
		PRIMITIVE_TYPE primitiveType;
		//ATTRIB_FORMAT_NONE denotes end of array, unless
		//using maximum number of attribs
		VertexDesc   attribs[MAX_ATTRIBS];
		U32          numVertices;
		BUFFER_USAGE usage;

		static U32 AttribSize(ATTRIB_FORMAT);
		U32 VertexSize() const;
		U32 Offset(ATTRIB_SEMANTIC) const;
	};

	class VertexBuffer {
	public:
		VertexBuffer(const VertexBufferDesc&);
		virtual ~VertexBuffer();

		const VertexBufferDesc &GetDesc() const { return m_desc; }

		/// get data pointer for modifying the buffer
		template <typename T>
		T* Map() {
			return reinterpret_cast<T*>(MapInternal());
		}
		/// finish modifications and upload to gpu
		virtual void Unmap() = 0;

	protected:
		virtual U8 *MapInternal() = 0;
		VertexBufferDesc m_desc;
	};
}