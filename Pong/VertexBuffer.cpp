#include "renderer/VertexBuffer.h"

void VertexBuffer::init( const void *data, GLuint size, GLenum usage )
{
	glGenBuffers( 1, &renderer_id_ );
	bind();
	glBufferData( GL_ARRAY_BUFFER, size, data, usage );
}

void VertexBuffer::shutDown()
{
	glDeleteBuffers( 1, &renderer_id_ );
}

void VertexBuffer::setData( const void *data, size_t size, GLuint offset )
{
	bind();
	glBufferSubData( GL_ARRAY_BUFFER, offset, size, data );
}

void VertexBuffer::bind() const
{
	glBindBuffer( GL_ARRAY_BUFFER, renderer_id_ );
}

void VertexBuffer::unbind() const
{
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

VertexBuffer::VertexBuffer() {}
VertexBuffer::~VertexBuffer() { shutDown(); }