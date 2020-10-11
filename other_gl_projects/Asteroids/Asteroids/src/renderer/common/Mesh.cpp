#include "Mesh.h"

Mesh::Mesh()
{
	vao_ = 0;
	vbo_ = 0;
	ibo_ = 0;
	count_ = 0;
}

Mesh::Mesh( const std::vector<Vertex>& vertices, const std::vector<GLushort>& indices )
{
	count_ = indices.size();

	glGenVertexArrays( 1, &vao_ );
	glGenBuffers( 1, &vbo_ );
	glGenBuffers( 1, &ibo_ );

	glBindVertexArray( vao_ );

	glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( Vertex ), &vertices[ 0 ], GL_STATIC_DRAW );
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo_ );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( GLushort ), &indices[ 0 ], GL_STATIC_DRAW );

	// Enable vertex positions
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void *)0 );

	// Enable color
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void *)offsetof(Vertex, color) );

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

Mesh::Mesh( Mesh &&other ) : vao_( other.vao_ ), vbo_( other.vbo_ ), ibo_( other.ibo_ ), count_( other.count_ )
{
	other.vao_ = 0;
	other.vbo_ = 0;
	other.ibo_ = 0;
	other.count_ = 0;
}

Mesh& Mesh::operator=( Mesh &&other )
{
	if( this != &other )
	{
		shutDown();
		vao_ = other.vao_;
		vbo_ = other.vbo_;
		ibo_ = other.ibo_;
		count_ = other.count_;

		other.vao_ = 0;
		other.vbo_ = 0;
		other.ibo_ = 0;
		other.count_ = 0;
	}

	return *this;
}

void Mesh::shutDown()
{
	glDeleteVertexArrays( 1, &vao_ );
	glDeleteBuffers( 1, &vbo_ );
	glDeleteBuffers( 1, &ibo_ );
}

GLsizei Mesh::getCount() const
{
	return count_;
}

GLuint Mesh::getVAO() const
{
	return vao_;
}

Mesh::~Mesh() { shutDown(); }