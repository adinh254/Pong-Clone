#ifndef WORLD_H
#define WORLD_H

#include "Components.h"
#include "Movement.h"

#include <GL/glew.h>
#include <SDL.h>
#include <array>
#include <stack>
#include <vector>


struct Entity
{
	GLuint index : 24;
	GLuint version : 8;
};

class World 
{
	public:
		World( GLfloat screen_width, GLfloat screen_height );
		Entity createEntity();
		void destroyEntity( Entity entity_id );

		std::vector<Entity> getScene() const;

		// TODO entity factory class
		//template<typename T>
		//void addComponent( Entity entity, const T& component )
		//{
		//	assert( false );
		//}

		//template<>
		//void addComponent<Renderable>( Entity entity, const Renderable& component )
		//{
		//	pool_.mask[entity] |= COMPONENT_RENDER;
		//	pool_.renders[ entity ] = component;
		//}
		//
		//template<>
		//void addComponent<Transform>( Entity entity, const Transform& component )
		//{
		//	pool_.mask[ entity ] |= COMPONENT_TRANSFORM;
		//	pool_.transforms[ entity ] = component;
		//}


		//template<>
		//void addComponent<Movement>( Entity entity, const Movement& component )
		//{
		//	pool_.mask[ entity ] |= COMPONENT_MOVEMENT;
		//	pool_.velocity[ entity ] = component;
		//}
		//const std::array<Renderable, MAX_ENTITIES>& getScene() const;

		~World();

	private:
		std::vector<Entity> pool_;
		std::stack<Entity, std::vector<Entity>> freelist_;

		GLuint entity_count_ = 0;
		GLfloat screen_width_, screen_height_;
		static const GLuint MAX_ENTITIES = 200;
		Movement move;
};

#endif