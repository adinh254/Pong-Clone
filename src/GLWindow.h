#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include "SDL_Deleter.h"

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <string>

class GLWindow
{
	public:
		static GLfloat WINDOW_WIDTH;
		static GLfloat WINDOW_HEIGHT;

		bool init( const std::string& title, GLfloat width, GLfloat height );
		SDL_Window* getWindow() const;

		// TODO: Handle size rechanged window event
		void handleEvents( SDL_Event& e );

		glm::mat4 getProjectionMatrix() const;

		GLWindow();
		~GLWindow();

	private:
		/// <summary>
		/// Window wrapper
		/// </summary>
		std::unique_ptr<SDL_Window, SDL_Deleter> createWindow( const char *title, int x, int y, int w, int h, Uint32 flags );

		std::unique_ptr<SDL_Window, SDL_Deleter> window_;
};

#endif