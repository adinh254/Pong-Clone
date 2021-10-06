#ifndef BALL_H
#define BALL_H

#include "Entity.h"
#include "CollisionHandler.h"

#include <SDL.h>
#include <GL/glew.h>
#include <glm/gtx/norm.hpp>

#include <random>

enum class BallState
{
	MOVING,
	PLAYER_1_WIN,
	PLAYER_2_WIN
};

class Ball
{
	public:
		// Init ball into the middle of the screen
		void init();
		GLuint getID() const;

		void setPosition( const glm::vec2& position );
		void setVelocity( const glm::vec2& velocity );
		void setModifier( const glm::vec2& velocity ); // Set velocity with Speed Cap and Speed Floor modifiers

		GLfloat getWidth() const;
		GLfloat getHeight() const;

		void setBallState( BallState state );
		BallState getBallState() const;

		void move();
		void trace();
		void setTrace( GLfloat velocity ); // Set tracer speed multiplier property

		glm::mat4 getPosition() const;
		glm::vec3 getPositionVector() const;
		glm::vec2 getVelocityVector() const;
		
		Collider getCollider() const;

		// Spawn ball in a random direction
		void spawn();

		// Spawn Ball in a direction using collider trajectory
		void spawn( const Collider& ball );

		Ball();
		~Ball();
	private:
		// Position offsets
		glm::vec3 position_vector_;

		// Current velocity of ball
		glm::vec2 velocity_vector_;

		// Current speed magnitude of ball
		GLfloat speed_;

		// Collision box
		Collider collider_;

		BallState ball_state_;

		// Window bounds
		GLfloat window_width_;
		GLfloat window_height_;

		GLfloat trace_multiplier_; // Tracer Modifier

		// Ball properties
		static const GLfloat DEFAULT_SPEED;
		static const GLfloat BALL_WIDTH;
		static const GLfloat BALL_HEIGHT;
		static const GLfloat SPEED_CEILING;
		static const GLfloat SPEED_FLOOR;

		const GLuint ID_; // Entity ID
		
};




#endif