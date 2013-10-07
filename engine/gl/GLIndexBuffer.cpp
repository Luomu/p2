#include "engine/gl/GLIndexBuffer.h"
#include "engine/OS.h"

using namespace Video;

GLIndexBuffer::GLIndexBuffer(U16 size, BUFFER_USAGE hint)
{
	m_size = size;

	const GLenum usage = (hint == BUFFER_STATIC) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	m_data = new U16[size];
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16) * m_size, m_data, usage);
}

GLIndexBuffer::~GLIndexBuffer()
{
	glDeleteBuffers(1, &m_buffer);
	delete[] m_data;
}

U16 *GLIndexBuffer::Map()
{
	return m_data;
}

void GLIndexBuffer::Unmap()
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(U16) * m_size, m_data);
}

void GLIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
}

void GLIndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}