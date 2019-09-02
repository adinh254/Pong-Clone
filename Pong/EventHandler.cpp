#include "EventHandler.h"

void EventHandler::registerEvent( SDL_EventType type, const EventCallback& callback )
{
	registered_events_[ type ].push_back( callback );
}

void EventHandler::handleEvents()
{
	while( SDL_PollEvent( &event_ ) != 0 )
	{
		for( const auto &callback : registered_events_[ event_.type ] )
		{
			callback( event_ );
		}
	}
}

SDL_Event EventHandler::getEvent() const
{
	return event_;
}