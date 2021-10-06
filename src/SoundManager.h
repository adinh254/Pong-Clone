#ifndef SOUND_MANAGER_H
#define	SOUND_MANAGER_H

#include "SDL_Deleter.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include <memory>
#include <string>

class SoundManager
{
	public:
		bool init();
		void shutDown();

		bool loadFX( const std::string& path );
		void playFX() const;

		SoundManager();
		~SoundManager();
	private:
		std::unique_ptr<Mix_Chunk, SDL_Deleter> createChunk( const char *path );

		std::unique_ptr<Mix_Chunk, SDL_Deleter> sfx_;
};


#endif

#include <SDL_mixer.h>