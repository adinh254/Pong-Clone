#include "Ball.h"

const GLfloat Ball::DEFAULT_SPEED = 3.0f;
const GLfloat Ball::BALL_WIDTH = 10.0f;
const GLfloat Ball::BALL_HEIGHT = 10.0f;
const GLfloat Ball::SPEED_CEILING = 6.0f;
const GLfloat Ball::SPEED_FLOOR = 1.0f;

void Ball::init()
{
	window_width_ = GLWindow::WINDOW_WIDTH;
	window_height_ = GLWindow::WINDOW_HEIGHT;

	spawn();
	trace_multiplier_ = 1.0f;
	ball_state_ = BallState::MOVING;
}

GLuint Ball::getID() const
{
	return ID_;
}

void Ball::move()
{
	position_vector_.x += velocity_vector_.x;
	position_vector_.y += velocity_vector_.y;

	collider_ = { position_vector_.x, position_vector_.y, BALL_WIDTH, BALL_HEIGHT };
	collider_.velocity = velocity_vector_;

	// Spawn another ball if it goes out of bounds
	if( ( position_vector_.x > window_width_ + BALL_WIDTH ) ) // Player_1 Victory
	{
		ball_state_ = BallState::PLAYER_1_WIN;
	}

	if( ( position_vector_.x + BALL_WIDTH ) < 0.0f ) // Player_2 Victory
	{
		ball_state_ = BallState::PLAYER_2_WIN;
	}

	if( ( position_vector_.y + BALL_HEIGHT ) < 0.0f ) 
	{
		position_vector_.y = 1.0f;
		collider_.y = position_vector_.y;
	}
	else if( position_vector_.y > window_height_ )
	{
		position_vector_.y = ( window_height_ - BALL_HEIGHT ) - 2.0f;
		collider_.y = position_vector_.y;
	}
}


void Ball::trace()
{
	position_vector_.x += velocity_vector_.x;
	position_vector_.y += velocity_vector_.y;

	collider_ = { position_vector_.x, position_vector_.y, BALL_WIDTH, BALL_HEIGHT };
	collider_.velocity = velocity_vector_;

	if( ( position_vector_.y + BALL_HEIGHT ) < 0.0f )
	{
		position_vector_.y = 1.0f;
		collider_.y = position_vector_.y;
	}
	else if( position_vector_.y > window_height_ )
	{
		position_vector_.y = ( window_height_ - BALL_HEIGHT ) - 2.0f;
		collider_.y = position_vector_.y;
	}
}

void Ball::setVelocity( const glm::vec2& velocity )
{
	velocity_vector_ = velocity;
	collider_.velocity = velocity;
}

void Ball::setBallState( BallState state )
{
	ball_state_ = state;
}

BallState Ball::getBallState() const
{
	return ball_state_;
}

void Ball::setPosition( const glm::vec2& pos )
{
	position_vector_ = vec3( pos.x, pos.y, 0.0f );

	collider_.x = pos.x;
	collider_.y = pos.y;
}

void Ball::setModifier( const glm::vec2& velocity )
{
	GLfloat abs_x = abs( velocity.x );
	GLfloat abs_y = abs( velocity.y );
	glm::vec2 modifier = velocity;

	if( abs_x > SPEED_CEILING ) 
	{
		if( velocity.x < 0.0f ) modifier.x = -SPEED_CEILING;
		else modifier.x = SPEED_CEILING;
	}
	else if( abs_x < SPEED_FLOOR )
	{
		if( velocity.x < 0.0f ) modifier.x = -SPEED_FLOOR;
		else modifier.x = SPEED_FLOOR;
	}

	if( abs_y > SPEED_CEILING )
	{
		if( velocity.y < 0.0f ) modifier.y = -SPEED_CEILING;
		else modifier.y = SPEED_CEILING;
	}

	velocity_vector_ = modifier;
	collider_.velocity = velocity_vector_;
}

void Ball::setTrace( GLfloat velocity )
{
	trace_multiplier_ = velocity;
}

glm::mat4 Ball::getPosition() const
{
	return glm::translate( glm::mat4( 1.0f ), position_vector_ );
}

glm::vec3 Ball::getPositionVector() const
{
	return position_vector_;
}

glm::vec2 Ball::getVelocityVector() const
{
	return velocity_vector_;
}

GLfloat Ball::getWidth() const
{
	return BALL_WIDTH;
}

GLfloat Ball::getHeight() const
{
	return BALL_HEIGHT;
}

Collider Ball::getCollider() const
{
	return collider_;
}

void Ball::spawn()
{
	// Position of ball is at the center of the window
	position_vector_ = glm::vec3( ( window_width_ - BALL_WIDTH ) / 2.0f, ( window_height_ - BALL_HEIGHT ) / 2.0f, 0.0f );
	collider_ = { position_vector_.x, position_vector_.y, BALL_WIDTH, BALL_HEIGHT };
	std::mt19937 rng( SDL_GetTicks() );
	std::uniform_int_distribution<GLuint> dist_x( 0, (GLuint)window_width_ );
	std::uniform_int_distribution<GLuint> dist_y( 0, (GLuint)window_height_ );

	glm::vec2 normalized;
	glm::vec2 random_velocity;
	GLfloat random_x = (GLfloat)dist_x( rng ) - ( window_width_ / 2 );
	GLfloat random_y = (GLfloat)dist_y( rng ) - ( window_height_ / 2 );
	glm::vec2 magnitude_vector = glm::vec2( random_x, random_y );
	normalized = normalize( magnitude_vector );
	random_velocity = normalized * DEFAULT_SPEED;

	velocity_vector_ = random_velocity; // Multiply speed by normalized vector to get velocity
	collider_.velocity = velocity_vector_;
}

void Ball::spawn( const Collider& ball )
{
	position_vector_.x = ball.x;
	position_vector_.y = ball.y;
	velocity_vector_.x = ball.velocity.x * trace_multiplier_;
	velocity_vector_.y = ball.velocity.y * trace_multiplier_;

	collider_ = { ball.x, ball.y, BALL_WIDTH, BALL_HEIGHT };
	collider_.velocity = velocity_vector_;
}

Ball::Ball() : ID_( Entity::ID++ ){ }
Ball::~Ball() {}