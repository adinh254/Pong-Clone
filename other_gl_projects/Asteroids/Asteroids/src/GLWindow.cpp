#include "GLWindow.h"

GLWindow::GLWindow( const std::string& title, GLfloat width, GLfloat height )
{
	title_ = title;
	width_ = width;
	height_ = height;
}

bool GLWindow::init()
{
	window_ = createWindow( title_.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)width_, (int)height_, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL );
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

glm::mat4 GLWindow::getProjection() const
{
	return glm::ortho( 0.0f, width_, height_, 0.0f, -1.0f, 1.0f );
}

GLfloat GLWindow::getWidth() const
{
	return width_;
}

GLfloat GLWindow::getHeight() const
{
	return height_;
}

std::unique_ptr<SDL_Window, SDL_Deleter> GLWindow::createWindow( const char* title, int x, int y, int w, int h, Uint32 flags )
{
	return std::unique_ptr<SDL_Window, SDL_Deleter>( SDL_CreateWindow( title, x, y, w, h, flags ), SDL_Deleter() );
}

GLWindow::~GLWindow() {}