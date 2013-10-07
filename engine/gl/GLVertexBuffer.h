#pragma once
#include "engine/VertexBuffer.h"

namespace Video {
	class GLVertexBuffer : public VertexBuffer {
	public:
		GLVertexBuffer(const VertexBufferDesc&);
		~GLVertexBuffer();

		/// get data pointer for modifying the buffer
		virtual U8 *Map();
		/// finish modifications and upload to gpu
		virtual void Unmap();

		void PrepareForDraw();

	private:
		void SetupAttribPointers();

		GLuint m_buffer;
		GLuint m_vao;
		U8* m_data;
	};
}