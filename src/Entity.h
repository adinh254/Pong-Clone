#ifndef	ENTITY_H
#define ENTITY_H

#include "GLWindow.h"
#include "CollisionHandler.h"

#include <SDL.h>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Entity
{
	public:
		static GLuint ID;

		void init();

		void setPosition( GLfloat x, GLfloat y );

		void move();

		GLfloat getX() const;
		GLfloat getY() const;
		GLfloat getWidth() const;
		GLfloat getHeight() const;
		GLuint getID() const;

		void setWidth( GLfloat width );
		void setHeight( GLfloat height );

		GLuint getNumOfEntities() const;

		/// <summary>
		/// Return current Entity model position
		/// </summary>
		/// <returns>A 4x4 model matrix</returns>
		glm::mat4 getPosition() const;

		/// <summary>
		/// For Collision Handler
		/// </summary>
		/// <returns></returns>
		Collider getCollider() const;

		/// <summary>
		/// Player Entity functions
		/// </summary>
		/// <param name="keyboard_state"></param>
		void handleInput( const Uint8* keyboard_state );
		void handleInputAlt( const Uint8* keyboard_state );
		void handleInputAI( GLfloat direction );

		static const GLfloat PLAYER_WIDTH;
		static const GLfloat PLAYER_HEIGHT;
		static const GLfloat PLAYER_SPEED;

		Entity();
		~Entity();

	private:
		GLfloat x_;
		GLfloat y_;

		GLfloat window_height_;
		GLfloat width_;
		GLfloat height_;

		glm::vec2 velocity_;
		const GLuint ID_;

		Collider collider_;
};


#endif