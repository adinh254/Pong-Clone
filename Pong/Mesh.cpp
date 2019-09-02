#include "Mesh.h"

template<typename T>
void Mesh<T>::init( const std::vector<T>& vertices, const std::vector<GLuint>& indices, const BufferLayout& layout, GLenum usage )
{
	vertices_ = vertices;
	indices_ = indices;
	layout_ = layout;

	setup( usage );
}

template<typename T>
void Mesh<T>::shutDown()
{
	vbo_.shutDown();
	vao_.shutDown();
	ibo_.shutDown();
}

template<typename T>
bool Mesh<T>::idExists( GLuint id ) const
{
	if( instance_ids_.find( id ) != instance_ids_.end() )
	{
		return true;
	}

	return false;
}

template<typename T>
void Mesh<T>::addInstanceID( GLuint id )
{
	instance_ids_[ id ]++;
}

template<typename T>
void Mesh<T>::addColor( const glm::vec4& color )
{
	colors_.push_back( color );
}

template<typename T>
void Mesh<T>::addModel( const glm::mat4& model )
{
	models_.push_back( model );
}

template<typename T>
void Mesh<T>::setModel( GLuint id, const glm::mat4& model )
{
	for( int i = 0; i < instance_ids_[ id ]; i++ )
	{
		models_[ id ] = model;
	}
}

template<typename T>
void Mesh<T>::setLayout( const BufferLayout& layout )
{
	layout_ = layout;
}

template<typename T>
glm::mat4 Mesh<T>::getModelByID( GLuint id ) const
{
	return models_[ id ];
}

template<typename T>
std::vector<glm::mat4> Mesh<T>::getModels() const
{
	return models_;
}

template<typename T>
std::vector<glm::vec4> Mesh<T>::getColors() const
{
	return colors_;
}

template<typename T>
void Mesh<T>::setVertices( const std::vector<T>& vertices, GLuint offset )
{
	vbo_.setData( &vertices[ 0 ], sizeof( T ), offset );
	vertices_ = vertices;
	vbo_.unbind();
}

template<typename T>
const std::vector<T>& Mesh<T>::getVertices() const
{
	return vertices_;
}

template<typename T>
GLuint Mesh<T>::getModelSize( GLuint id ) const
{
	return instance_ids_.at( id );
}


template<typename T>
void Mesh<T>::renderInstances() const
{
	vao_.bind();
	glDrawElementsInstanced( GL_TRIANGLES, ibo_.getCount(), GL_UNSIGNED_INT, nullptr, models_.size() );
}

template<typename T>
void Mesh<T>::render() const
{
	vao_.bind();
	glDrawElements( GL_TRIANGLES, ibo_.getCount(), GL_UNSIGNED_INT, nullptr );
}

template<typename T>
void Mesh<T>::setup( GLenum usage )
{
	vao_.init();
	vbo_.init( &vertices_[ 0 ], vertices_.size() * sizeof( T ), usage );
	ibo_.init( &indices_[ 0 ], indices_.size() );

	vao_.addBuffer( vbo_, layout_ );

	vao_.unbind();
	vbo_.unbind();
	ibo_.unbind();
}

template<typename T>
Mesh<T>::Mesh() {}

template<typename T>
Mesh<T>::~Mesh() { shutDown(); }

template class Mesh<Vertex>;
template class Mesh<TexVertex>;