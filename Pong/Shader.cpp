#include "renderer/Shader.h"

bool Shader::init( const std::string& vertex_path, const std::string& fragment_path )
{
	renderer_id_ = 0;
	std::string vertex_source = parseShader( vertex_path, GL_VERTEX_SHADER );
	std::string fragment_source = parseShader( fragment_path, GL_FRAGMENT_SHADER );

	if( vertex_source.empty() && fragment_source.empty() ) return false;

	renderer_id_ = createShader( vertex_source, fragment_source );
	if( !renderer_id_ ) return false; // Shader failed to compile

	bind();

	return true;
}

void Shader::shutDown() const
{
	glDeleteProgram( renderer_id_ );
}

void Shader::bind() const
{
	glUseProgram( renderer_id_ );
}

void Shader::unbind() const
{
	glUseProgram( 0 );
}

GLuint Shader::createShader( const std::string& vertex_shader_src, const std::string& fragment_shader_src ) const
{
	GLuint program_id = glCreateProgram();

	GLuint vertex_shader = compileShader( GL_VERTEX_SHADER, vertex_shader_src );
	GLuint fragment_shader = compileShader( GL_FRAGMENT_SHADER, fragment_shader_src );

	if( !( vertex_shader || fragment_shader ) ) return 0;

	glAttachShader( program_id, vertex_shader );
	glAttachShader( program_id, fragment_shader );
	glLinkProgram( program_id );
	glValidateProgram( program_id );

	deleteShaders( vertex_shader, fragment_shader );

	return program_id;
}

GLuint Shader::compileShader( GLuint shader_type, const std::string& source ) const
{
	GLuint shader_id = glCreateShader( shader_type );
	auto src = std::make_unique<const char*>( source.c_str() );
	glShaderSource( shader_id, 1, src.get(), nullptr );
	glCompileShader( shader_id );

	GLint shader_compiled = GL_FALSE;
	glGetShaderiv( shader_id, GL_COMPILE_STATUS, &shader_compiled );
	if( shader_compiled != GL_TRUE )
	{
		std::string shader_error;
		switch( shader_type )
		{
			case GL_VERTEX_SHADER:
				shader_error = " Failed to compile vertex shader ";
				break;
			case GL_FRAGMENT_SHADER:
				shader_error = " Failed to compile fragment shader  ";
				break;
		}

		SDL_Log( "%s! Shader ID %s\n", shader_error.c_str(), getShaderLog(shader_id).c_str() );
		glDeleteShader( shader_id );

		return 0;
	}

	return shader_id;
}

void Shader::setUniform4fArray( const std::string& name, const std::vector<glm::vec4>& vec4 )
{
	// Insert into cache
	for( unsigned int i = 0; i < vec4.size(); i++ )
	{
		std::string uniform = name + "[" + std::to_string( i ) + "]";
		auto cache_it = uniform_cache_.find( uniform );

		if( cache_it == uniform_cache_.end() )
		{
			uniform_cache_[ uniform ] = getUniformLocation( uniform );
		}
	}
	glUniform4fv( uniform_cache_[ name + "[0]" ], vec4.size(), glm::value_ptr( vec4[ 0 ] ) );
}

void Shader::setUniformMat4fByLoc( const std::string& name, const std::vector<glm::mat4>& matrices, GLuint id )
{
	std::string uniform = name + "[" + std::to_string(id) + "]";
	auto cache_it = uniform_cache_.find( uniform );
	if( cache_it != uniform_cache_.end() )
	{
		GLint location = cache_it->second;
		glUniformMatrix4fv( location, matrices.size(), GL_FALSE, glm::value_ptr( matrices[ 0 ] ) );
	}
	else
	{
		SDL_Log( "Cannot find ID in cache!\n" );
	}
}

void Shader::setUniformMat4fArray( const std::string& name, const std::vector<glm::mat4>& matrices )
{
	for( unsigned int i = 0; i < matrices.size(); i++ )
	{
		std::string uniform = name + "[" + std::to_string( i ) + "]";
		auto cache_it = uniform_cache_.find( uniform );
		if( cache_it == uniform_cache_.end() )
		{
			uniform_cache_[ uniform ] = getUniformLocation( uniform );
		}
	}
	glUniformMatrix4fv( uniform_cache_[ name + "[0]" ], matrices.size(), GL_FALSE, glm::value_ptr( matrices[ 0 ] ) );
}

void Shader::setUniformMat4f( const std::string& name, const glm::mat4& matrix )
{
	auto cache_it = uniform_cache_.find( name );
	if( cache_it == uniform_cache_.end() )
	{
		uniform_cache_[ name ] = getUniformLocation(name);
	}
	glUniformMatrix4fv( uniform_cache_[name], 1, GL_FALSE, glm::value_ptr( matrix ) );
}

void Shader::setUniform4f( const std::string& name, const glm::vec4& vec )
{
	auto cache_it = uniform_cache_.find( name );
	if( cache_it == uniform_cache_.end() )
	{
		uniform_cache_[ name ] = getUniformLocation( name );
	}
	glUniform4fv( uniform_cache_[ name ], 1, glm::value_ptr( vec ) );
}

GLint Shader::getUniformLocation( const std::string& name) const
{
	GLint location = glGetUniformLocation( renderer_id_, name.c_str() );
	if( location == -1 )
		SDL_Log( "Uniform %s doesn't exist!\n", name.c_str() );

	return location;
}

std::string Shader::parseShader( const std::string& file_path, GLuint shader_type ) const
{
	std::ifstream in_file( file_path );

	std::string line;

	if( in_file.is_open() )
	{
		getline( in_file, line );
		if( line.find( "#shader" ) != std::string::npos )
		{
			switch( shader_type )
			{
				// Check if the right shaders are called
				case GL_VERTEX_SHADER:
				{
					if( line.find( "vertex" ) == std::string::npos )
					{
						SDL_Log( "Incompatible shader type! Shader is not a vertex shader!\n" );
						return  "";
					}
					break;
				}
				case GL_FRAGMENT_SHADER:
				{
					if( line.find( "fragment" ) == std::string::npos )
					{
						SDL_Log( "Incompatible shader type! Shader is not a fragment shader!\n" );
						return  "";
					}
					break;
				}
				default:
				{
					SDL_Log( "Incompatible shader type! Shader is not a fragment or vertex shader!\n" );
					return "";
				}
			}
		}
		else
		{
			SDL_Log( "File %s is not a shader!\n", file_path.c_str() );
			return "";
		}
	}
	else
	{
		SDL_Log( "Failed to load file! Does file %s exist?\n", file_path.c_str() );
		return "";
	}

	std::stringstream buffer;
	buffer << in_file.rdbuf();

	return buffer.str();
}


std::string Shader::getShaderLog( GLuint id ) const
{
	std::string info_log;

	if( glIsShader( id ) || glIsProgram( id ) )
	{
		int info_log_length = 0;
		int max_length = info_log_length;

		// Get info string length
		glGetShaderiv( id, GL_INFO_LOG_LENGTH, &max_length );

		// Allocate string
		char *info_log_c = new char[ max_length ];

		// Get log
		glGetShaderInfoLog( id, max_length, &info_log_length, info_log_c );
		info_log = info_log_c;

		// Deallocate char array
		delete[] info_log_c;

		return info_log;
	}
	info_log = "Name " + std::to_string( id ) +  " is not a shader or program.\n";
	
	return info_log;
}

void Shader::deleteShaders(GLuint vertex_shader, GLuint fragment_shader) const
{
	glDetachShader( renderer_id_, vertex_shader );
	glDetachShader( renderer_id_, fragment_shader );

	glDeleteShader( vertex_shader );
	glDeleteShader( fragment_shader );
}

Shader::Shader() {}
Shader::~Shader() { shutDown(); }