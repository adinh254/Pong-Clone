#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H	

#include <SDL.h>

#include <functional>
#include <vector>
#include <map>

class EventHandler
{
	public:
		using EventCallback = std::function<void( const SDL_Event& )>;

		void registerEvent( SDL_EventType event_type, const EventCallback& callback );
		void handleEvents();
	private:
		std::map<Uint32, std::vector<EventCallback>> registered_;
		SDL_Event event_;
};

#endif