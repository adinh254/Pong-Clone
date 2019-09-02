#ifndef COMPUTER_H
#define COMPUTER_H

#include "Entity.h"
#include "CollisionHandler.h"

#include "glm/glm.hpp"

class Computer
{
	public:
		void init( const Collider& paddle );
		void react( const Collider& ball );

		void setPos( GLfloat y );
		void setSpeed( GLfloat speed );
		GLfloat getSpeed() const;
		GLfloat getInput() const;

		Computer();
		~Computer();

	private:
		void predict( GLfloat ball_pos_y, GLfloat ball_h );

		Collider paddle_;
		GLfloat input_;

		GLfloat speed_;
};

#endif