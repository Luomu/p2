#pragma once
#include "Common.h"

namespace Video {
	class IndexBuffer {
	public:
		virtual ~IndexBuffer();

		virtual U16 *Map() = 0;
		virtual void Unmap() = 0;

		U16 GetSize() const { return m_size; }

	protected:
		U16 m_size;
	};
}