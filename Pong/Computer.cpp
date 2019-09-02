#include "Computer.h"

void Computer::init( const Collider& paddle )
{
	paddle_ = paddle;

	input_ = 0.0f;
}

void Computer::setPos( GLfloat y)
{
	paddle_.y = y;
}

void Computer::setSpeed( GLfloat speed )
{
	speed_ = speed;
}

GLfloat Computer::getSpeed() const
{
	return speed_;
}

void Computer::react( const Collider& ball )
{
	if( ( 0.0f - ball.velocity.x ) <= 0.0f ) // Only move if ball is going towards player
	{
		predict( ball.y, ball.h );
	}
	else
		input_ = 0.0f;
}

void Computer::predict( GLfloat ball_pos_y, GLfloat ball_h )
{
	input_ = 0.0f;
	if( ball_pos_y < paddle_.y )
		input_ = -speed_;
	else if( ( ball_pos_y + ball_h ) > ( paddle_.y + paddle_.h ) )
		input_ = speed_;
}

GLfloat Computer::getInput() const
{
	return input_;
}

Computer::Computer() {}
Computer::~Computer() {}