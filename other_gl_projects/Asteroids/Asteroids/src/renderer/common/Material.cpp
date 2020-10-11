#include "Material.h"

Material::Material()
{
	shader_id_ = 0;
}

Material::Material( const std::string& vert_path, const std::string& frag_path )
{
	std::string vert_source;
	std::string frag_source;

	std::ifstream vert_file( vert_path );
	std::ifstream frag_file( frag_path );

	if( vert_file.is_open() && frag_file.is_open() )
	{
		std::stringstream vert_stream, frag_stream;
		vert_stream << vert_file.rdbuf();
		frag_stream << frag_file.rdbuf();

		vert_file.close();
		frag_file.close();

		vert_source = vert_stream.str();
		frag_source = frag_stream.str();
		const GLchar* vert_code = vert_source.c_str();
		const GLchar* frag_code = frag_source.c_str();

		// Compile shaders
		GLuint vert_id, frag_id;
		GLint compiled;

		// Logger
		GLint max_length = 0;
		std::vector<GLchar> error_log;

		vert_id = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( vert_id, 1, &vert_code, nullptr );
		glCompileShader( vert_id );

		// Logger
		glGetShaderiv( vert_id, GL_COMPILE_STATUS, &compiled );
		if( !compiled ) // If shader fails to compile
		{
			glGetShaderiv( vert_id, GL_INFO_LOG_LENGTH, &max_length ); // Get maximum length of returned buffer log
			error_log.resize( max_length );

			glGetShaderInfoLog( vert_id, max_length, &max_length, &error_log[ 0 ] );

			SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Vertex Shader Compilation Error:%s\n",
				std::string( begin( error_log ), end( error_log ) ).c_str() );
		}

		frag_id = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( frag_id, 1, &frag_code, nullptr );
		glCompileShader( frag_id );

		glGetShaderiv( frag_id, GL_COMPILE_STATUS, &compiled );
		if( !compiled )
		{
			glGetShaderiv( vert_id, GL_INFO_LOG_LENGTH, &max_length ); // Get maximum length of returned buffer log
			error_log.resize( max_length );

			glGetShaderInfoLog( vert_id, max_length, &max_length, &error_log[ 0 ] );

			SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Fragment Shader Compilation Error:%s\n",
				std::string( begin( error_log ), end( error_log ) ).c_str() );
		}

		shader_id_ = glCreateProgram();
		glAttachShader( shader_id_, vert_id );
		glAttachShader( shader_id_, frag_id );
		glLinkProgram( shader_id_ );
		// Link Error Log
		GLint linked;
		glGetProgramiv( shader_id_, GL_LINK_STATUS, &linked );
		if( !linked )
		{
			glGetProgramiv( shader_id_, GL_INFO_LOG_LENGTH, &max_length );
			error_log.resize( max_length );
			glGetProgramInfoLog( shader_id_, max_length, &max_length, &error_log[ 0 ] );

			SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Shader Program Linking Error:%s\n",
				std::string( begin( error_log ), end( error_log ) ).c_str() );
		}
		glDeleteShader( vert_id );
		glDeleteShader( frag_id );
	}
	else
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to read Shaders!\n" );
	}
}

Material::Material( Material &&other ) : shader_id_( other.shader_id_ )
{
	other.shader_id_ = 0;
}

Material& Material::operator=( Material &&other )
{
	if( this != &other )
	{
		shutDown();
		shader_id_ = other.shader_id_;
		shader_id_ = 0;
	}

	return *this;
}

GLuint Material::getShaderID() const
{
	return shader_id_;
}

void Material::shutDown()
{
	glDeleteProgram( shader_id_ );
}

Material::~Material()
{ shutDown(); }