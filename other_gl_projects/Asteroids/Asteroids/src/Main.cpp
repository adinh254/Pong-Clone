#include "Game.h"

int main( int argc, char *argv[] )
{
	Game game;
	if( !game.run( "Asteroids", 640.0f, 480.0f ) ) return -1;
	game.shutDown();

	return 0;
}