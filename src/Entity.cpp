#include "Entity.h"

const GLfloat Entity::PLAYER_WIDTH = 10.0f;
const GLfloat Entity::PLAYER_HEIGHT = 60.0f;
const GLfloat Entity::PLAYER_SPEED = 5.0f;
GLuint Entity::ID = 0;

void Entity::init()
{
	x_ = 0.0f;
	y_ = 0.0f;

	velocity_ = glm::vec2( 0.0f, 0.0f );
	width_ = PLAYER_WIDTH;
	height_ = PLAYER_HEIGHT;

	window_height_ = GLWindow::WINDOW_HEIGHT;

	collider_.x = x_;
	collider_.y = y_;
	collider_.w = width_;
	collider_.h = height_; 
	collider_.velocity = velocity_;

}

GLfloat Entity::getX() const
{
	return x_;
}

GLfloat Entity::getY() const
{
	return y_;
}

void Entity::setPosition( GLfloat x, GLfloat y )
{
	x_ = x;
	y_ = y;

	collider_.x = x_;
	collider_.y = y_;
}

GLuint Entity::getID() const
{
	return ID_;
}

glm::mat4 Entity::getPosition() const
{
	return glm::translate( glm::mat4( 1.0f ), glm::vec3( x_, y_, 0.0f ) );
}

void Entity::setWidth( GLfloat width )
{
	width_ = width;
	collider_.w = width;
}

void Entity::setHeight( GLfloat height )
{
	height_ = height;
	collider_.h = height;
}

GLfloat Entity::getWidth() const
{
	return width_;
}

GLfloat Entity::getHeight() const
{
	return height_;
}

void Entity::move()
{
	// Move paddle up or down
	y_ += velocity_.y;

	// Check if Entity has moved out of bounds
	if( y_ < 0.0f )
		y_ -= y_;
	else if( ( y_ + PLAYER_HEIGHT ) > window_height_ )
		y_ = window_height_ - PLAYER_HEIGHT;

	collider_.x = x_;
	collider_.y = y_;

	collider_.velocity = velocity_;
}

void Entity::handleInput( const Uint8* keyboard_state )
{
	if( keyboard_state[ SDL_SCANCODE_UP ] )
		velocity_.y = -PLAYER_SPEED;
	else if( keyboard_state[ SDL_SCANCODE_DOWN ] )
		velocity_.y = PLAYER_SPEED;
	else
		velocity_.y = 0.0f;

	collider_.velocity = velocity_;
}

void Entity::handleInputAlt( const Uint8* keyboard_state )
{
	if( keyboard_state[ SDL_SCANCODE_W ] )
		velocity_.y = -PLAYER_SPEED;
	else if( keyboard_state[ SDL_SCANCODE_S ] )
		velocity_.y = PLAYER_SPEED;
	else
		velocity_.y = 0.0f;

	collider_.velocity = velocity_;
}

void Entity::handleInputAI( GLfloat direction )
{
	velocity_.y = direction;
	collider_.velocity = velocity_;
}

Collider Entity::getCollider() const
{
	return collider_;
}

GLuint Entity::getNumOfEntities() const
{
	return ID;
}

Entity::Entity() : ID_(ID++) {}
Entity::~Entity() {}