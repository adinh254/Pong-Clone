#include "World.h"

World::World( GLfloat screen_width, GLfloat screen_height )
{
	screen_width_ = screen_width;
	screen_height_ = screen_height;
}

Entity World::createEntity()
{
	if( !freelist_.empty() )
	{
		Entity entity = freelist_.top();
		freelist_.pop();
		return entity;
	}

	return { entity_count_++, 0 };
}

void World::destroyEntity( Entity entity_id )
{
	if( entity_id.index >= 0 && entity_id.index < MAX_ENTITIES )
	{
		pool_[ entity_id.index ].version++;
		freelist_.push( entity_id );
	}

}

//std::vector<Entity> World::getScene() const
//{
//	std::vector<Entity> renderables;
//	for( const auto& entity : pool_ )
//	{
//		if( entity.renderable_id != 0 )
//		{
//			renderables.push_back( entity );
//		}
//	}
//
//	return renderables;
//}



World::~World() {}