#include "Game.h"

bool Game::run( const std::string& title, GLfloat width, GLfloat height )
{
	if( !initSDL() ) return false;
	if (!window_.init( title, width, height )) return false;
	if( !initGL()) return false;
	if ( !loadAssets() ) return false;

	Computer computer;
	computer.init( player_2_.getCollider() );

	sound_.loadFX( "audio/hitsound.wav" );
	glm::mat4 projection = window_.getProjectionMatrix();

	running_ = true;
	paused_ = false;

	std::string msg;
	GLfloat msg_size;

	GLfloat title_size = text_renderer_.getTextWidth( title );
	std::string player_1_msg = "Player 1 Wins";
	std::string player_2_msg = "Player 2 Wins";
	std::string resume_msg = "Press Enter to Play Again";
	GLfloat player_1_msg_size = text_renderer_.getTextWidth( player_1_msg );
	GLfloat player_2_msg_size = text_renderer_.getTextWidth( player_2_msg );
	GLfloat resume_msg_size = text_renderer_.getTextWidth( resume_msg );

	// Init event handler
	EventHandler handler;
	// Register game events to handler
	registerEventHandler( handler );

	// Register ball as current collider
	collision_handler_.init( ball_.getCollider() );

	GLuint player_1_id = player_1_.getID();
	GLuint player_2_id = player_2_.getID();
	GLuint ball_id = ball_.getID();
	GLuint tracer_id = tracer_.getID();

	GLuint player_1_score = 0;
	GLuint player_2_score = 0;

	glm::mat4 ball_scale = glm::scale( glm::mat4( 1.0f ), glm::vec3( ball_.getWidth() / player_1_.getWidth(), ball_.getHeight() / player_1_.getHeight(), 0.0f ) );

	// Load Menu
	menu_.init( text_renderer_ );
	menu_.handleEvents( handler );
	
	// Game loop
	while( isRunning() )
	{
		// Event Loop
		handler.handleEvents();

		// Clear buffer
		glClear( GL_COLOR_BUFFER_BIT );

		// Input handling
		const Uint8* current_key_states = SDL_GetKeyboardState( nullptr );

		// Menu loop
		if( !menu_.isFinished() )
		{
			menu_.handleInput( current_key_states );
			computer.setSpeed( menu_.getCompProperties().speed );
			tracer_.setTrace( menu_.getCompProperties().prediction );

			// Render Loop
			GLfloat padding = 0.0f;

			text_renderer_.renderText( title, ( GLWindow::WINDOW_WIDTH - title_size ) / 2.0f, GLWindow::WINDOW_HEIGHT * 0.15f, 1.0f, mesh_.getColors()[ 0 ], false );
			padding = ( GLWindow::WINDOW_HEIGHT - ( GLWindow::WINDOW_HEIGHT * 0.15f ) ) / 2.0f;
			for( size_t i = 0; i < menu_.getButtons().size(); i++ )
			{
				Button choice = menu_.getButtons()[ i ];
				if( i == menu_.getCurrentChoice() )
				{
					text_renderer_.renderText( choice.text, ( ( GLWindow::WINDOW_WIDTH - ( choice.width / 2.0f ) ) / 2.0f ), padding, 0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), false );
				}
				else
					text_renderer_.renderText( choice.text,  ( ( GLWindow::WINDOW_WIDTH -  ( choice.width / 2.0f ) ) / 2.0f ), padding, 0.5f, mesh_.getColors()[ 0 ], false );
				padding += ( text_renderer_.getFontHeight() / 2.0f ) + Menu::TEXT_PADDING;
			}
		}
		else
		{
			shader_.bind();
			mesh_.renderInstances();
			if( !paused_ )
			{
				player_1_.handleInputAlt( current_key_states ); // Set velocity

				if( menu_.getCompProperties().speed > 0.0f ) // Single Player
				{
					player_2_.handleInputAI( computer.getInput() ); 
					computer.setPos( player_2_.getY() );
					computer.react( tracer_.getCollider() );
				}
				else // 2 Player
					player_2_.handleInput( current_key_states ); // Set velocity

				// Set collider cache
				collision_handler_.setSurface( player_1_.getCollider(), player_1_id );
				// Set collider cache
				collision_handler_.setSurface( player_2_.getCollider(), player_2_id );

				// Collision Detection for ball
				collision_handler_.setCollider( ball_.getCollider() );
				collision_handler_.handleCollision();
				if( collision_handler_.getState() != CollideState::NONE )
				{
					ball_.setPosition( collision_handler_.getPos() );
					ball_.setModifier( collision_handler_.getDeflectAngle() );

					if( collision_handler_.getState() == CollideState::DEFLECT )
						tracer_.spawn( ball_.getCollider() );

					sound_.playFX();
				}
				else
				{
					ball_.move();
				}
				// Collision Detection for tracer
				collision_handler_.setCollider( tracer_.getCollider() );
				collision_handler_.handleCollision();
				if( collision_handler_.getState() != CollideState::NONE )
				{
					tracer_.setPosition( collision_handler_.getPos() );
					tracer_.setVelocity( collision_handler_.getDeflectAngle() );
					if( collision_handler_.getState() == CollideState::STICKY )
					{
						tracer_.setVelocity( glm::vec2( 0.0f, 0.0f ) );
					}
				}
				else
				{
					tracer_.trace();
					if( tracer_.getPositionVector().x > ( ( player_2_.getX() - player_2_.getWidth() ) - 1.0f ) ) // Stop tracer if it goes over Paddle 2 y pos
					{
						tracer_.setVelocity( glm::vec2( 0.0f, 0.0f ) );
					}
				}

				// Move paddles
				player_1_.move();
				player_2_.move();

				// Set mesh model of player 1
				glm::mat4 player_1_model = player_1_.getPosition();

				// Set mesh model of player 2
				glm::mat4 player_2_model = player_2_.getPosition();

				// Set mesh model of ball
				glm::mat4 ball_model = ball_.getPosition() * ball_scale;

				// Tracer
				// glm::mat4 tracer_model = tracer_.getPosition() * ball_scale;

				entity_mvps_.mvps[ player_1_id ] = projection * player_1_model;
				entity_mvps_.mvps[ player_2_id ] = projection * player_2_model;
				entity_mvps_.mvps[ ball_id ] = projection * ball_model;
				// entity_mvps_.mvps[ tracer_id ] = projection * tracer_model;

				switch( ball_.getBallState() )
				{
					case( BallState::PLAYER_1_WIN ):
					{
						player_1_score++;
						ball_.spawn();
						ball_.setBallState( BallState::MOVING );
						tracer_.setPosition( ball_.getPositionVector() );
						tracer_.setVelocity( ball_.getVelocityVector() );
						break;
					}
					case ( BallState::PLAYER_2_WIN ):
					{
						player_2_score++;
						ball_.spawn();
						ball_.setBallState( BallState::MOVING );
						tracer_.setPosition( ball_.getPositionVector() );
						tracer_.setVelocity( ball_.getVelocityVector() );
						break;
					}
					default:
						break;
				}
				if( player_1_score > 10 )
				{
					msg = player_1_msg;
					msg_size = player_1_msg_size;
					paused_ = true;
				}
				else if( player_2_score > 10 )
				{
					msg = player_2_msg;
					msg_size = player_2_msg_size;
					paused_ = true;
				}

			}
			else
			{
				if( current_key_states[ SDL_SCANCODE_RETURN ] )
				{
					paused_ = false;
					player_1_score = 0;
					player_2_score = 0;

					player_1_.setPosition( 15.0f, ( GLWindow::WINDOW_HEIGHT - player_1_.getHeight() ) / 2 );
					player_2_.setPosition( ( GLWindow::WINDOW_WIDTH - player_1_.getWidth() ) - 15.0f, ( GLWindow::WINDOW_HEIGHT - player_1_.getHeight() ) / 2 );
					ball_.spawn();
					tracer_.setPosition( ball_.getPositionVector() );
					tracer_.setVelocity( ball_.getVelocityVector() );
				}
				text_renderer_.renderText( msg, ( GLWindow::WINDOW_WIDTH - ( msg_size / 2.0f ) ) / 2.0f, 100.0F, 0.5f, mesh_.getColors()[ 0 ], false );
				text_renderer_.renderText( resume_msg, ( GLWindow::WINDOW_WIDTH - ( resume_msg_size / 2.0f ) ) / 2.0f, 320.0F, 0.5f, mesh_.getColors()[ 0 ], false );
			}

			// Set model view projection of entities of player_1 and player_2
			shader_.setUniformMat4fByLoc( "u_MVP", entity_mvps_.mvps, player_1_id );

			// Render all objects using shader

			text_renderer_.renderText( std::to_string( player_1_score ), ( GLWindow::WINDOW_WIDTH / 2 ) - 40.0F, 10.0F, 0.5f, mesh_.getColors()[ 0 ], true );
			text_renderer_.renderText( std::to_string( player_2_score ), ( GLWindow::WINDOW_WIDTH / 2 ) + 20.0F, 10.0F, 0.5f, mesh_.getColors()[ 0 ], false );
		}
		
		update();
	}

	return true;
}

void Game::registerEventHandler( EventHandler& handler )
{
	// Register escape button on event handler
	handler.registerEvent( SDL_KEYUP, [ this ] ( const SDL_Event& e ) { 
		if( e.key.keysym.sym == SDLK_ESCAPE )
		{
			menu_.overlay();
			ball_.spawn();
			player_1_.setPosition( 15.0f, ( GLWindow::WINDOW_HEIGHT - player_1_.getHeight() ) / 2 );
			player_2_.setPosition( ( GLWindow::WINDOW_WIDTH - player_1_.getWidth() ) - 15.0f, ( GLWindow::WINDOW_HEIGHT - player_1_.getHeight() ) / 2 );
			tracer_.setPosition( ball_.getPositionVector() );
			tracer_.setVelocity( ball_.getVelocityVector() );
		}
	} );

	// Register SDL_QUIT event on event handler
	handler.registerEvent( SDL_QUIT, [ this ] ( const SDL_Event& e ) {
		running_ = false;
	} );
}

void Game::update()
{
	SDL_GL_SwapWindow( window_.getWindow() );
}

bool Game::initSDL()
{
	running_ = false;
	SDL_version v;

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL! SDL Error: ", SDL_GetError() );
		return false;
	}

	if( !sound_.init() )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize Sound Manager!\n" );
		return false;
	}

	running_ = true;

	SDL_Log( "SDL Initialized!\n" );
	SDL_VERSION( &v );
	SDL_Log( "Compiled against SDL %u.%u.%u", v.major, v.minor, v.patch );

	SDL_GetVersion( &v );
	SDL_Log( "Using SDL %u.%u.%u", v.major, v.minor, v.patch );

	running_ = true;
	return true;
}

bool Game::initGL()
{
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 ); // OpenGL 3+
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 ); // OpenGl 3.3
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE ); // OpenGL core profile

	gl_context_ = SDL_GL_CreateContext( window_.getWindow() );
	if( gl_context_ == nullptr )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize OpenGL context! SDL Error: %s", SDL_GetError() );
		return false;
	}

	SDL_Log( "Initialized OpenGL" );
	SDL_Log( "Using OpenGl %s", glGetString( GL_VERSION ) );

	// Iniialize GLEW
	GLenum glew_error = glewInit();
	if( glew_error != GLEW_OK )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize glew! Glew Error: %s\n", glewGetErrorString( glew_error ) );
		return false;
	}
	// Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		SDL_Log( "Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	// Enable Transparency
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	return true;
}

bool Game::loadAssets()
{
	GLfloat window_width = GLWindow::WINDOW_WIDTH;
	GLfloat window_height = GLWindow::WINDOW_HEIGHT;

	// Load text renderer
	if( !text_renderer_.init( window_.getProjectionMatrix() ) )
	{
		SDL_LogWarn( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize text renderer!\n" );
		return false;
	}
	else
	{
		if( !text_renderer_.loadTTF( "./fonts/bit5x3.ttf", 96 ) )
		{
			SDL_LogWarn( SDL_LOG_CATEGORY_APPLICATION, "Failed to load font face!\n" );
			return false;
		}
	}

	// Game asset colors
	glm::vec4 crt_green = glm::vec4( 0.254f, 1.0f, 0.0f, 1.0f );
	glm::vec4 see_thru_green = glm::vec4( 0.254f, 1.0f, 0.0f, 0.4f );

	// Init Players & Dynamic Entities and add to collision handler cache
	player_1_.init();
	player_1_.setPosition( 15.0f, ( window_height - player_1_.getHeight() ) / 2 );
	glm::mat4 player_1_model = player_1_.getPosition();
	collision_handler_.addSurface( player_1_.getCollider() );
	mesh_.addColor( crt_green );

	player_2_.init();
	player_2_.setPosition( ( window_width - player_1_.getWidth() ) - 15.0f, ( window_height - player_1_.getHeight() ) / 2 );
	glm::mat4 player_2_model = player_2_.getPosition();
	collision_handler_.addSurface( player_2_.getCollider() );
	mesh_.addColor( crt_green );

	// Init ball
	ball_.init();
	glm::mat4 ball_scale = glm::scale( glm::mat4( 1.0f ), glm::vec3( ball_.getWidth() / player_1_.getWidth(), ball_.getHeight() / player_1_.getHeight(), 0.0f ) );
	glm::mat4 ball_model = ball_.getPosition() * ball_scale;
	mesh_.addColor( crt_green );

	tracer_.init();
	tracer_.setPosition( ball_.getPositionVector() );
	tracer_.setVelocity( ball_.getVelocityVector() );
	// mesh_.addColor( glm::vec4( 0.254f, 0.0f, 1.0f, 1.0f ) );

	glm::mat4 wall_scale = glm::scale( glm::mat4( 1.0f ), glm::vec3( ( window_width / player_1_.getWidth() ), 1 / (player_1_.getHeight()), 0.0f ) );

	Entity wall_1;
	wall_1.init();
	wall_1.setPosition( 0.0f, 0.0f );
	wall_1.setWidth( window_width );
	wall_1.setHeight( 1.0f );
	glm::mat4 wall_1_model = wall_1.getPosition() * wall_scale;
	collision_handler_.addSurface( wall_1.getCollider() );
	mesh_.addColor( crt_green );

	Entity wall_2;
	wall_2.init();
	wall_2.setPosition( 0.0f, window_height - 1.0f );
	wall_2.setWidth( window_width );
	wall_2.setHeight( 1.0f );
	glm::mat4 wall_2_translation = glm::translate( glm::mat4( 1.0f ), glm::vec3( wall_2.getX(), wall_2.getY(), 0.0f ) );
	glm::mat4 wall_2_model = wall_2_translation * wall_scale;
	collision_handler_.addSurface( wall_2.getCollider() );
	mesh_.addColor( crt_green );


	// Object Buffer layout
	BufferLayout layout;
	// Position attribute
	layout.push<GLfloat>( 2 );

	// Set Paddle vertices
	std::vector<Vertex> object_vertices = {
		{ glm::vec2( 0.0f,				   0.0f					 ) }, // Top left
		{ glm::vec2( player_1_.getWidth(), 0.0f					 ) }, // Top right
		{ glm::vec2( player_1_.getWidth(), player_1_.getHeight() ) }, // Bottom right
		{ glm::vec2( 0.0f,				   player_1_.getHeight() ) } // Bottom Left
	};

	std::vector<GLuint> object_indices = { 0, 1, 2, 2, 3, 0 };

	glm::mat4 projection = window_.getProjectionMatrix();

	// Initialize meshes with a VBO, VAO, and IBO
	mesh_.init( object_vertices, object_indices, layout, GL_STATIC_DRAW );

	// Add default position model matrices to model vector and set the instance id and current number of models to render
	mesh_.addModel( player_1_model );
	mesh_.addModel( player_2_model );
	mesh_.addModel( ball_model );

	// mesh_.addModel( ball_model ); // Tracer Model

	mesh_.addModel( wall_1_model );
	mesh_.addModel( wall_2_model );

	mesh_.addInstanceID( player_1_.getID() );
	mesh_.addInstanceID( player_2_.getID() );
	mesh_.addInstanceID( ball_.getID() );

	// mesh_.addInstanceID( tracer_.getID() ); // Tracer ID

	mesh_.addInstanceID( wall_1.getID() );
	mesh_.addInstanceID( wall_2.getID() );

	// Set mid line using player settings
	Entity mid_line;
	mid_line.init();

	// Middle line is half the scale of a paddle
	glm::mat4 mid_line_scale = glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.5f, 0.5f, 0.0f ) );
	GLfloat line_height = mid_line.getHeight() * 0.5f;
	// Spacing between each line
	GLfloat line_spacing = 4.0f;
	GLfloat num_of_lines = window_height / ( line_height + line_spacing );
	GLfloat line_pos_x = ( window_width - (mid_line.getWidth() * 0.5f ) ) / 2.0f;

	// Offset to vertically align line to the center 
	GLfloat line_offset_y = 2.0f;
	GLuint mid_line_id = mid_line.getID();

	// Insert mid_line object into the instance array
	if( !mesh_.idExists( mid_line_id ) )
	{
		// Mid_line position model matrices
		for( int i = 0; i < ( num_of_lines ); i++ )
		{
			GLfloat line_pos_y = ( (GLfloat)i * ( line_height + line_spacing ) ) + line_offset_y;
			glm::mat4 mid_line_translation = glm::translate( glm::mat4( 1.0f ), glm::vec3( line_pos_x, line_pos_y, 0.0f ) );

			// Model matrix = Translation * scale
			glm::mat4 mid_line_model = mid_line_translation * mid_line_scale;
			mesh_.addModel( mid_line_model );
			mesh_.addInstanceID( mid_line_id );
			// Set object colors
			mesh_.addColor( see_thru_green );
		}
	}

	auto models = mesh_.getModels();
	// Convert all default game models matrices to default model view projection matrices
	std::transform( models.begin(), models.end(), std::back_inserter( default_mvps_.mvps ), [ projection ] ( const auto& model ) {
		return ( projection * model );
	} );

	// Register all dynamic entity objects in cache
	entity_mvps_.mvps.push_back( default_mvps_.mvps[ player_1_.getID() ] );
	entity_mvps_.mvps.push_back( default_mvps_.mvps[ player_2_.getID() ] );

	// Register ball in cache
	entity_mvps_.mvps.push_back( default_mvps_.mvps[ ball_.getID() ] );

	// Register tracer in cache (DEBUG)
	// entity_mvps_.mvps.push_back( default_mvps_.mvps[ tracer_.getID() ] );

	// Initialize all Shader program and bind to renderer
	if( !shader_.init( "./shaders/shader.vertex", "./shaders/shader.fragment" ) )
	{
		SDL_LogWarn( SDL_LOG_CATEGORY_APPLICATION, "Failed to load default shader!\n" );
		return false;
	}
	else
	{
		shader_.setUniform4fArray( "u_Color", mesh_.getColors() );
		// Set default model view projections to be rendered first
		shader_.setUniformMat4fArray( "u_MVP", default_mvps_.mvps );
		shader_.unbind();
	}

	return true;
}

bool Game::isRunning() const
{
	return running_;
}

void Game::shutDown()
{
	sound_.shutDown();
	mesh_.shutDown();
	text_renderer_.shutDown();
	shader_.shutDown();

	SDL_Quit();
	SDL_GL_DeleteContext( gl_context_ );
}


Game::Game() {}
Game::~Game() {}