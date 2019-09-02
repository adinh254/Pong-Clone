#include "renderer/BufferLayout.h"

template<typename T>
void BufferLayout::push( GLuint count )
{
	static_assert( sizeof( T ) == -1, "You need a specialization for Push!" );
}

template<>
void BufferLayout::push<GLfloat>( GLuint count )
{
	elements_.push_back( { count, GL_FLOAT, GL_FALSE } );
	stride_ += sizeof( GLfloat ) * count;
}

template<>
void BufferLayout::push<GLuint>( GLuint count )
{
	elements_.push_back( { count, GL_UNSIGNED_INT, GL_FALSE } );
	stride_ += sizeof( GLuint ) * count;
}

const std::vector<BufferElement>& BufferLayout::getElements() const 
{
	return elements_; 
}

const GLsizei BufferLayout::getStride() const 
{ 
	return stride_; 
}

template void BufferLayout::push<GLfloat>( GLuint count );
template void BufferLayout::push<GLuint>( GLuint count );


BufferLayout::BufferLayout() { stride_ = 0; }
BufferLayout::~BufferLayout() {}