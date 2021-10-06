#ifndef SDL_DELETER_H
#define SDL_DELETER_H

#include <SDL.h>
#include <SDL_mixer.h>

struct SDL_Deleter
{
	void operator()( SDL_Window *p ) const { SDL_DestroyWindow( p ); }
	void operator()( Mix_Chunk *p ) const { Mix_FreeChunk( p ); }
};

#endif