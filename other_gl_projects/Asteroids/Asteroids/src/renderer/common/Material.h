#ifndef MATERIAL_H
#define MATERIAL_H

#include <SDL.h>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Material
{
	public:
		Material();
		Material( const std::string& vert_path, const std::string& frag_path );

		Material( const Material &material ) = delete;
		Material& operator=( const Material &material ) = delete;

		Material( Material &&other );
		Material& operator=( Material &&other );

		GLuint getShaderID() const;
		void shutDown();

		~Material();
	private:
		GLuint shader_id_;
};

#endif