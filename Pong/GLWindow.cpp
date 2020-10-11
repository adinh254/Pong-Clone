#include "GLWindow.h"

GLfloat GLWindow::WINDOW_WIDTH;
GLfloat GLWindow::WINDOW_HEIGHT;

bool GLWindow::init(const std::string& title, GLfloat width, GLfloat height )
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	window_ = createWindow( title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int) WINDOW_WIDTH, (int) WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
	if( window_ == nullptr )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Failed to create window! SDL Error: %s", SDL_GetError() );
		return false;
	}

	return true;
}

SDL_Window* GLWindow::getWindow() const
{
	return window_.get();
}


//void GLWindow::handleEvents( EventHandler& handler ) 
//{
//	switch( e.window.event )
//	{
//		case SDL_WINDOWEVENT_SIZE_CHANGED:
//	}
//}

glm::mat4 GLWindow::getProjectionMatrix() const
{
	return glm::ortho( 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f );
}

std::unique_ptr<SDL_Window, SDL_Deleter> GLWindow::createWindow( const char *title, int x, int y, int w, int h, Uint32 flags )
{
	return std::unique_ptr<SDL_Window, SDL_Deleter>( SDL_CreateWindow( title, x, y, w, h, flags ), SDL_Deleter() );
}

GLWindow::GLWindow() {}
GLWindow::~GLWindow() {}
