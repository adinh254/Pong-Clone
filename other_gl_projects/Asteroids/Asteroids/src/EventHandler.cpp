#include "EventHandler.h"

void EventHandler::registerEvent( SDL_EventType event_type, const EventCallback& callback )
{
	registered_[ event_type ].push_back( callback );
}

void EventHandler::handleEvents()
{
	while( SDL_PollEvent( &event_ ) != 0 )
	{
		for( const auto& callback : registered_[ event_.type ] )
		{
			callback( event_ );
		}
	}
}