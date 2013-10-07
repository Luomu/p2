#include "engine/gl/GLVertexBuffer.h"
#include "engine/OS.h"

using namespace Video;

namespace {
	GLenum MapType(ATTRIB_FORMAT format) {
		switch (format) {
			case ATTRIB_FORMAT_FLOAT2:
			case ATTRIB_FORMAT_FLOAT3:
			case ATTRIB_FORMAT_FLOAT4:
				return GL_FLOAT;
			case ATTRIB_FORMAT_UBYTE4:
				return GL_UNSIGNED_BYTE;
			case ATTRIB_FORMAT_NONE:
			default:
				return GL_NONE; //invalid
		}
	}

	GLint MapNumComponents(ATTRIB_FORMAT format) {
		switch (format) {
			case ATTRIB_FORMAT_FLOAT2:
				return 2;
			case ATTRIB_FORMAT_FLOAT3:
				return 3;
			case ATTRIB_FORMAT_FLOAT4:
				return 4;
			case ATTRIB_FORMAT_UBYTE4:
				return 4;
			case ATTRIB_FORMAT_NONE:
			default:
				return 0; //invalid
		}
	}
}

GLVertexBuffer::GLVertexBuffer(const VertexBufferDesc &desc)
: VertexBuffer(desc)
{
	glGenBuffers(1, &m_buffer);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

	//allocate initial data store
	const U32 vtxSize = desc.VertexSize();
	const U32 dataSize = vtxSize * desc.numVertices;
	m_data = new U8[dataSize];
	memset(m_data, 0, vtxSize);
	const GLenum usage = (desc.usage == BUFFER_STATIC) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
	glBufferData(GL_ARRAY_BUFFER, dataSize, m_data, usage);

	SetupAttribPointers();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLVertexBuffer::~GLVertexBuffer()
{
	glDeleteBuffers(1, &m_buffer);
	glDeleteVertexArrays(1, &m_vao);
	delete[] m_data;
}

void GLVertexBuffer::SetupAttribPointers()
{
	U32 stride = m_desc.VertexSize();
	GLsizei offset = 0;
	for (U8 i = 0; i < MAX_ATTRIBS; i++) {
		if (m_desc.attribs[i].semantic == ATTRIB_NONE) break;
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, MapNumComponents(m_desc.attribs[i].format),
			MapType(m_desc.attribs[i].format), GL_TRUE, stride, (GLvoid*)offset);
		offset += m_desc.AttribSize(m_desc.attribs[i].format);
	}
}

U8 *GLVertexBuffer::Map()
{
	return m_data;
}

void GLVertexBuffer::Unmap()
{
	const GLsizei dataSize = m_desc.numVertices * m_desc.VertexSize();
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, m_data);
}

void GLVertexBuffer::PrepareForDraw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBindVertexArray(m_vao);
}