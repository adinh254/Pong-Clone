#ifndef GAME_H
#define GAME_H

#include "renderer/Shader.h"
#include "renderer/BufferLayout.h"
#include "GLWindow.h"
#include "Entity.h"
#include "Ball.h"
#include "Computer.h"
#include "EventHandler.h"
#include "Mesh.h"
#include "TextRenderer.h"
#include "SoundManager.h"
#include "Menu.h"

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <stack>

struct ModelViewProjections
{
	std::vector<glm::mat4> mvps;
};

class Game
{
	public:
		bool run( const std::string& title, GLfloat width, GLfloat height );
		void update();
		bool loadAssets();

		void registerEventHandler( EventHandler& handler );
		/// <summary>
		/// Quit SDL and OpenGL context
		/// </summary>
		void shutDown();

		Game();
		~Game();

	private:
		/// <summary>
		/// Initializes SDL, SDL_OpenGl, glew and setups the window
		/// </summary>
		/// <param name="title">The window title</param>
		bool initSDL();

		/// <summary>
		/// Initializes OpenGL and setup a shader, vertex buffer, and vertex array object
		/// </summary>
		bool initGL();

		/// <summary>
		/// Check if game is running
		/// </summary>
		/// <returns></returns>
		bool isRunning() const;

		GLWindow window_;
		SDL_GLContext gl_context_;

		// Default Model View Projection matrices of all game objects
		ModelViewProjections default_mvps_;

		// Entity Model View Projection cache for dynamic entities
		ModelViewProjections entity_mvps_;

		// Mesh for object rendering
		Mesh<Vertex> mesh_;

		// Text Renderer
		TextRenderer text_renderer_;

		// Default Shader
		Shader shader_;

		// Text Renderer
		TextRenderer text_;

		// Sound Manager
		SoundManager sound_;
		// Player objects
		Entity player_1_;
		Entity player_2_;

		// Main Menu
		Menu menu_;

		// Ball
		Ball ball_;
		Ball tracer_;

		// Collision Handler
		CollisionHandler collision_handler_;

		bool running_;
		bool paused_;
};

#endif // !WINDOW_H
