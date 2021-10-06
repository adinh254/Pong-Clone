#include "renderer/VertexArray.h"

void VertexArray::init()
{
	glGenVertexArrays( 1, &renderer_id_ );
	bind();
}

void VertexArray::shutDown()
{
	glDeleteVertexArrays( 1, &renderer_id_ );
}

void VertexArray::addBuffer( const VertexBuffer& vb, const BufferLayout& layout )
{
	vb.bind();
	const std::vector<BufferElement>& elements = layout.getElements();
	GLuint offset = 0;
	for( std::size_t i = 0; i < elements.size(); i++ )
	{
		const auto& element = elements[ i ];
		glEnableVertexAttribArray( i );
		glVertexAttribPointer( i, element.count, element.type, element.normalized, layout.getStride(), (const void*) offset );
		offset += element.count * sizeof( element.type );
	}
}

void VertexArray::bind() const
{
	glBindVertexArray( renderer_id_ );
}

void VertexArray::unbind() const
{
	glBindVertexArray( 0 );
}

VertexArray::VertexArray() {}
VertexArray::~VertexArray() { shutDown(); }