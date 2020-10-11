#include "Renderer.h"

Renderer::Renderer()
{
	projection_ = glm::mat4( 1.0f );
}

Renderer::Renderer( const glm::mat4& projection )
{
	projection_ = projection;
}

void Renderer::setProjection( const glm::mat4& projection )
{
	projection_ = projection;
}

void Renderer::addModel( const Model& model  )
{
	scene_.push_back( model );
}

void Renderer::render( const World& world )
{
	for( auto& model : scene_ )
	{
		model.mvps.clear(); // Clear all MVP matrices (retains memory)
	}

	for( const auto& renderable : world.getScene() )
	{
		if( renderable.vao_id != 0 )
		{
			glm::mat4 model_matrix = 
			glm::mat4 mvp = projection_ * renderable.model_matrix; // Gets mvp
			scene_[ renderable.vao_id - 1 ].mvps.push_back( mvp );
		}
	}

	// TODO: Geometry shader/Stencil buffer to draw outline only
	for( size_t i = 0; i < scene_.size(); i++ )
	{
		glBindVertexArray( scene_[ i ].mesh_id );
		glUseProgram( scene_[ i ].material_id );

		for( const auto& transformation : scene_[ i ].mvps )
		{
			GLsizei elements = scene_[ i ].elements;
			setUniformMat4f( scene_[ i ].material_id, "u_MVP", transformation );
			glDrawElements( GL_LINE_LOOP, elements, GL_UNSIGNED_SHORT, nullptr );
		}
	}
}

void Renderer::setUniformMat4f( GLuint shader_id, const std::string& name, const glm::mat4& matrix )
{
	GLint location;
	auto cache_it = uniforms_cache_.find( name );
	if( cache_it == uniforms_cache_.end() )
	{
		location = glGetUniformLocation( shader_id, name.c_str() );

		if( location == -1 )
		{
			GLint uniforms;
			glGetProgramiv( shader_id, GL_ACTIVE_UNIFORMS, &uniforms );
			SDL_LogWarn( SDL_LOG_CATEGORY_APPLICATION, "Uniform %s doesn't exist!\nNumber of Uniforms Active: %i", name.c_str(), uniforms );
		}
		else
			uniforms_cache_[ name ] = location;
	}
	else
		location = uniforms_cache_[ name ];

	glUniformMatrix4fv( location, 1, GL_FALSE, glm::value_ptr( matrix ) );
}

Renderer::~Renderer() {}