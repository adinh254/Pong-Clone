#ifndef MESH_H
#define MESH_H

#include <gl/glew.h>
#include <glm/glm.hpp>

#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
};

class Mesh
{
	public:
		Mesh();
		Mesh( const std::vector<Vertex>& vertices, const std::vector<GLushort>& indices );
		
		Mesh( const Mesh& mesh ) = delete;
		Mesh &operator=( const Mesh& mesh ) = delete;

		Mesh( Mesh &&other );
		Mesh &operator=( Mesh &&other );

		void shutDown();

		GLsizei getCount() const;

		GLuint getVAO() const;

		~Mesh();

	private:
		GLuint vao_, vbo_, ibo_;
		GLsizei count_;
};

#endif