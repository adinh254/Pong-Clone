#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <string>
#include <array>
#include <fstream>
#include <unordered_map>

struct ShaderSource
{
	std::string vertex_source;
	std::string fragment_source;
};

class Shader
{
	public:
		bool init( const std::string& vertex_path, const std::string& fragment_path );
		void shutDown() const;
		void bind() const;
		void unbind() const;

		/// <summary>
		/// Set vec4 uniform all at once by array
		/// </summary>
		/// <param name="name"></param>
		/// <param name="vec4"></param>
		void setUniform4fArray( const std::string& name, const std::vector<glm::vec4>& vec4 );
		/// <summary>
		/// Set uniform in order of ids
		/// </summary>
		/// <param name="name"></param>
		/// <param name="matrices"></param>
		/// <param name="id"></param>
		void setUniformMat4fByLoc( const std::string& name, const std::vector<glm::mat4>& matrices, GLuint id );
		/// <summary>
		/// Set mat4 uniform all at once by array
		/// </summary>
		/// <param name="name"></param>
		/// <param name="matrices"></param>
		void setUniformMat4fArray( const std::string& name, const std::vector<glm::mat4>& matrices );

		/// <summary>
		/// Set one mat4 uniform
		/// </summary>
		/// <param name="name"></param>
		/// <param name="matrix"></param>
		void setUniformMat4f( const std::string& name, const glm::mat4& matrix );

		/// <summary>
		/// Set one vec4 uniform
		/// </summary>
		void setUniform4f( const std::string& name, const glm::vec4& vec );

		Shader();
		~Shader();

	private:
		GLuint createShader( const std::string& vertex_shader, const std::string& fragment_shader ) const;
		GLuint compileShader( GLuint shader_type, const std::string& source ) const;
		GLint getUniformLocation( const std::string& name ) const;
		std::string parseShader( const std::string& file_path, GLuint shader_type ) const;

		void deleteShaders( GLuint vertex_shader, GLuint fragment_shader ) const;
		std::string getShaderLog( GLuint id ) const;

		// Caching
		std::unordered_map<std::string, GLint> uniform_cache_;
		// Variables
		GLuint renderer_id_;
};

#endif