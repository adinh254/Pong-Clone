#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include "SDLDeleter.h"

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <string>

class GLWindow
{
	public:
		GLWindow( const std::string& title, GLfloat width, GLfloat height );
		
		bool init();
		SDL_Window* getWindow() const;
		glm::mat4 getProjection() const;

		GLfloat getWidth() const;
		GLfloat getHeight() const;

		~GLWindow();

	private:
		/// <summary>
		/// Smart pointer window wrapper
		/// </summary>
		/// <param name="title"></param>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="h"></param>
		/// <param name="flags"></param>
		/// <returns></returns>
		std::unique_ptr<SDL_Window, SDL_Deleter> createWindow( const char* title, int x, int y, int w, int h, Uint32 flags );

		std::unique_ptr<SDL_Window, SDL_Deleter> window_;

		std::string title_;
		GLfloat width_;
		GLfloat height_;
};


#endif