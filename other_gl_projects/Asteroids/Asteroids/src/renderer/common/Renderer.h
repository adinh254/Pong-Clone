#include "Mesh.h"
#include "Material.h"

#include "src/World.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>
#include <map>
#include <array>

struct Model
{
	GLuint mesh_id, material_id;
	GLsizei elements;
	std::vector<glm::mat4> mvps;
};

class Renderer
{
	public:
		Renderer();
		Renderer( const glm::mat4& projection );

		void setProjection( const glm::mat4& projection );

		void addModel( const Model& model );

		void render( const World& world );

		~Renderer();

	private:
		void setUniformMat4f( GLuint shader_id, const std::string& name, const glm::mat4& matrix );
		void setColor( GLuint shader_id, const std::string& name, const glm::vec4& color );

		glm::mat4 projection_;
		std::vector<Model> scene_;
		std::unordered_map<std::string, GLint> uniforms_cache_;
};