#ifndef GAME_H
#define GAME_H
#include "GLWindow.h"
#include "EventHandler.h"

#include "World.h"

#include "renderer/common/Renderer.h"
#include "renderer/common/Material.h"
#include "renderer/common/Mesh.h"
#include "ModelLoader.h"

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SDL.h>

#include <string>

class Game
{
	public:
		Game();
		bool run( const std::string& title, GLfloat screen_width, GLfloat screen_height );
		void shutDown();

		void registerEventHandler( EventHandler& handler );

		~Game();
	private:
		bool initSDL();
		bool initGL( const GLWindow& window );

		bool running_;

		SDL_GLContext gl_context_;
};

#endif
