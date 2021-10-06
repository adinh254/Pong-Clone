#include "Game.h"

#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_opengl.h>

#include <iostream>
#include <string>
#include <vector>


int main( int argc, char *args[] )
{
	Game game;

	// TODO: Single Player Mode and 2-Player Mode
	if( !game.run( "Pong", 640.0f, 480.0f ) ) return -1;
	game.shutDown();

	return 0;
}