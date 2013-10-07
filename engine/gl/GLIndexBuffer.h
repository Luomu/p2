#pragma once
#include "engine/IndexBuffer.h"

namespace Video {
	class GLIndexBuffer : public IndexBuffer {
	public:
		GLIndexBuffer(U16 numIndices, BUFFER_USAGE);
		virtual ~GLIndexBuffer();

		virtual U16 *Map();
		virtual void Unmap();

		void Bind();
		void Unbind();
	private:
		GLuint m_buffer;
		U16 *m_data;
 	};
}