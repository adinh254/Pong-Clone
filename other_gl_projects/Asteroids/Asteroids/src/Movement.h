#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "Entities.h"
#include "Components.h"
#include "World.h"

#include <glm/glm.hpp>

struct Movement
{
	Transform transform;
	glm::vec3 velocity;
};

class Movement
{
	public:
		Movement();
		~Movement();

		void handleInput( Entity id, const Uint8* key_state );
		void move();

	private:
		std::vector<Entity> sparse_;
		std::vector<Transform> packed_;
};

#endif // MOVEMENT_H
