#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "Entity.h"

#include <SDL.h>

#include <functional>
#include <vector>
#include <map>

class EventHandler
{
	public:
		using EventCallback = std::function<void( const SDL_Event& )>;

		void registerEvent( SDL_EventType type, const EventCallback& callback );
		void handleEvents();
		SDL_Event getEvent() const;

	private:
		std::map<Uint32, std::vector<EventCallback>> registered_events_;
		SDL_Event event_;
};

#endif