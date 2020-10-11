#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <array>

//enum Mask
//{
//	COMPONENT_NONE = 0,
//	COMPONENT_TRANSFORM = 1 << 0,
//	COMPONENT_MOVEMENT = 1 << 1,
//	COMPONENT_HITBOX = 1 << 2,
//	COMPONENT_RENDER = 1 << 3
//};

struct Transform
{
	glm::vec3 position;
	GLfloat rotation;

	Transform( const glm::vec3& position, GLfloat rotation )
		: position( position )
		, rotation( rotation )
	{
		}
};
//
//struct Movement
//{
//	glm::vec3 velocity = glm::vec3( 0.0f );
//};
//

#endif