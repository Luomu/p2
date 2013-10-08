#pragma once
#include "engine/VertexBuffer.h"

namespace Video {
	class GLVertexBuffer : public VertexBuffer {
	public:
		GLVertexBuffer(const VertexBufferDesc&);
		~GLVertexBuffer();

		virtual void Unmap();

		void PrepareForDraw();

	protected:
		virtual U8 *MapInternal();

	private:
		void SetupAttribPointers();

		GLuint m_buffer;
		GLuint m_vao;
		U8* m_data;
	};
}