#ifndef MESH_H
#define MESH_H

#include "renderer/VertexArray.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/BufferLayout.h"
#include "renderer/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <unordered_map>

struct Vertex
{
	glm::vec2 pos;
};

struct TexVertex
{
	glm::vec2 pos;
	glm::vec2 tex_coords;
};

template<typename T>
class Mesh
{
	public:
		void init( const std::vector<T>& vertices, const std::vector<GLuint>& indices, const BufferLayout& layout, GLenum usage );
		void shutDown();

		void setLayout( const BufferLayout& layout );
		bool idExists( GLuint id ) const;

		void setVertices( const std::vector<T>& vertices, GLuint offset);
		const std::vector<T>& getVertices() const;

		/// <summary>
		/// Add instance id as key with current position as value
		/// </summary>
		/// <param name="id"></param>
		void addInstanceID( GLuint id );


		/// <summary>
		/// Return position of object id in the instance array only if it exists
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		GLuint getModelSize( GLuint id ) const;

		void addColor( const glm::vec4& color );
		void addModel( const glm::mat4& model );

		/// <summary>
		/// Set model matrix by entity size
		/// </summary>
		/// <param name="id"></param>
		/// <param name="model"></param>
		void setModel( GLuint id, const glm::mat4& model );
		std::vector<glm::mat4> getModels() const;
		std::vector<glm::vec4> getColors() const;

		/// <summary>
		/// Return first model of instance
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		glm::mat4 getModelByID( GLuint id ) const;

		void renderInstances() const;
		void render() const;

		Mesh();
		~Mesh();

	private:
		void setup( GLenum usage );

		VertexArray vao_;
		VertexBuffer vbo_;
		IndexBuffer ibo_;
		BufferLayout layout_;

		std::vector<T> vertices_;
		std::vector<GLuint> indices_;

		/// <summary>
		/// Stores id of model objects
		/// </summary>
		std::unordered_map<GLuint, GLsizei> instance_ids_;
		
		std::vector<glm::mat4> models_;
		std::vector<glm::vec4> colors_;

		glm::mat4 scale_;
		glm::mat4 translation_;
};
#endif // !MESH_H
