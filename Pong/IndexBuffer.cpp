#include "renderer/IndexBuffer.h"

void IndexBuffer::init( const void *index_data, GLuint count )
{
	count_ = count;
	glGenBuffers( 1, &renderer_id_ );
	bind();
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( GLuint ), index_data, GL_STATIC_DRAW );
}

void IndexBuffer::shutDown() const
{
	glDeleteBuffers( 1, &renderer_id_ );
}

GLuint IndexBuffer::getCount() const
{
	return count_;
}

void IndexBuffer::bind() const
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, renderer_id_ );
}

void IndexBuffer::unbind() const
{
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

IndexBuffer::IndexBuffer() { count_ = 0; }
IndexBuffer::~IndexBuffer() { shutDown(); }