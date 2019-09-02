#include "SoundManager.h"

bool SoundManager::init()
{
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 512 ) < 0 )
	{
		SDL_LogWarn( SDL_LOG_CATEGORY_APPLICATION, "SDL Mixer could not be initialized! SDL Error: %s\n", Mix_GetError() );
		return false;
	}

	return true;
}

bool SoundManager::loadFX( const std::string& path )
{
	sfx_ = createChunk( path.c_str() );
	if( sfx_ == nullptr )
	{
		SDL_LogWarn( SDL_LOG_CATEGORY_APPLICATION, "Failed to load %s SFX! SDL_mixer Error: %s\n", path, Mix_GetError() );
		return false;
	}
	return true;
}

void SoundManager::playFX() const
{
	Mix_PlayChannel( -1, sfx_.get(), 0 );
}

void SoundManager::shutDown()
{
	Mix_Quit();
}

std::unique_ptr<Mix_Chunk, SDL_Deleter> SoundManager::createChunk( const char *path )
{
	return std::unique_ptr<Mix_Chunk, SDL_Deleter>( Mix_LoadWAV( path ), SDL_Deleter() );
}

SoundManager::SoundManager() {}

SoundManager::~SoundManager() {}