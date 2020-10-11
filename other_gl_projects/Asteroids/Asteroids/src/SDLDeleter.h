#ifndef SDL_DELETER_H
#define SDL_DELETER_H

#include <SDL.h>

struct SDL_Deleter
{
	void operator()( SDL_Window *p ) const { SDL_DestroyWindow( p ); }
};

#endif