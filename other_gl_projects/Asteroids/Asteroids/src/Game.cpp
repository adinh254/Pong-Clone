#include "Game.h"

bool Game::run( const std::string& title, GLfloat screen_width, GLfloat screen_height )
{
	running_ = false;
	GLWindow window( title, screen_width, screen_height );

	// Initialize APIs
	if( !initSDL() ) return false;
	if( !window.init() ) return false;
	if( !initGL( window ) ) return false;

	// Initialize Game components
	EventHandler handler;
	World world( screen_width, screen_height );
	Renderer renderer( window.getProjection() );
	registerEventHandler( handler );
	
	// All entity render colors
	glm::vec4 entity_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	Material entity_mat( "resources/shader/vert.glsl", "resources/shader/frag.glsl" );

	// Initialize Player Model
	std::vector<Vertex> player_vertices;
	std::vector<GLushort> player_indices;
	if( !loadObj( "resources/models/ship.obj", player_vertices, player_indices ) ) return false;
	for( auto& vertex : player_vertices )
		vertex.color = entity_color;
	Mesh ship_mesh( player_vertices, player_indices );
	Model ship_model = { ship_mesh.getVAO(), entity_mat.getShaderID(), ship_mesh.getCount() };

	renderer.addModel( ship_model );
	glm::mat4 ship_pos = glm::translate( glm::mat4( 1.0f ), glm::vec3( screen_width / 2.0f, screen_height / 2.0f, 0.0f ) );
	glm::mat4 ship_scale = glm::scale( glm::mat4( 1.0f ), glm::vec3( 10.0f, 10.0f, 0.0f ) );
	

	running_ = true;

	// Core loop
	while( running_ )
	{
		handler.handleEvents();

		// Render loop
		renderer.render( world );
		SDL_GL_SwapWindow( window.getWindow() );
	}

	return true;
}

bool Game::initSDL()
{
	SDL_version v;

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL! SDL Error: %s ", SDL_GetError() );
		return false;
	}

	SDL_Log( "SDL Initialized!\n" );
	SDL_VERSION( &v );
	SDL_Log( "Compiled against SDL %u.%u.%u", v.major, v.minor, v.patch );

	SDL_GetVersion( &v );
	SDL_Log( "Using SDL %u.%u.%u", v.major, v.minor, v.patch );

	return true;
}

bool Game::initGL( const GLWindow& window )
{
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 ); // OpenGL 3+
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 ); // Opengl 3.3
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ); // OpenGL core profile

	// Create OpenGL context
	gl_context_ = SDL_GL_CreateContext( window.getWindow() );
	if( gl_context_ == nullptr )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize OpenGL context! SDL Error: %s", SDL_GetError() );
		return false;
	}

	SDL_Log( "Initialized OpenGL" );
	SDL_Log( "Using OpenGl %s", glGetString( GL_VERSION ) );

	// Initialize GLEW
	GLenum glew_error = glewInit();
	if( glew_error != GLEW_OK )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize glew! Glew Error: %s\n", glewGetErrorString( glew_error ) );
		return false;
	}

	// VSync On
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		SDL_Log( "Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	// Enable transparency
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	return true;
}

void Game::registerEventHandler( EventHandler& handler )
{
	handler.registerEvent( SDL_QUIT, [ this ] ( const SDL_Event& e ) {
		running_ = false;
	} );
}

void Game::shutDown()
{
	SDL_Quit();
	SDL_GL_DeleteContext( gl_context_ );
}

Game::Game() {}
Game::~Game() {}