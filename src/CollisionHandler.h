#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <vector>

using namespace glm;

enum class CollideState
{
	NONE,
	DEFLECT,
	STICKY,
	REFLECT
};

struct Collider
{
	GLfloat x;
	GLfloat y;
	GLfloat w;
	GLfloat h;
	glm::vec2 velocity;
};

struct MinkowskiDifference
{
	glm::vec2 entry;
	glm::vec2 exit;
};

/// <summary>
/// Swept AABB collision detection
/// </summary>
class CollisionHandler
{
	public:
		void init( const Collider& collider );
		void setCollider( const Collider& collider );

		/// <summary>
		/// Add all collidable surfaces to handler
		/// </summary>
		/// <param name="surface"></param>
		void addSurface( const Collider& surface );
		void setSurface( const Collider& surface, GLuint id );

		CollideState getState() const;

		glm::vec2 getPos() const;


		/// <summary>
		/// Broad phase handling
		/// </summary>
		/// <param name="collider"></param>
		/// <returns></returns>
		void handleCollision();

		MinkowskiDifference getDistance( const Collider& surface );

		glm::vec2 getDeflectAngle() const;

		CollisionHandler();
		~CollisionHandler();


	private:
		std::vector<Collider> surface_cache_;
		
		CollideState collide_state_;

		/// <summary>
		/// Response parameters
		/// </summary>
		vec2 deflect_angle_;
		Collider collider_;
		GLfloat time_of_collision_;
		vec2 surface_normal_;
		vec2 relative_;

		/// <summary>
		/// Calculates surface normal
		/// </summary>
		vec2 getSurfaceNormal( const  MinkowskiDifference& collision_times ) const;
		
		/// <summary>
		/// Returns the latest entry time
		/// </summary>
		/// <param name="collision_time"></param>
		/// <returns></returns>
		GLfloat getTimeOfCollision( const Collider& surface, const MinkowskiDifference& collision_times ) const;

		/// <summary>
		/// Calculates and returns entry time of collider and surface using Time = Distance / Velocity
		/// </summary>
		/// <param name="collider"></param>
		/// <param name="surface"></param>
		/// <param name="distance"></param>
		/// <returns></returns>
		MinkowskiDifference getCollisionTimes( const Collider& surface, const MinkowskiDifference& distance ) const;

		// Collision modifiers
		glm::vec2 getHitboxModifier( const Collider& paddle ) const;

		/// /// <summary>
		/// Calculate angle by taking the collision 
		/// </summary>
		/// <param name="surface_normal"></param>
		/// <param name="collider_vel"></param>
		void calculateDeflectAngle( const vec2& modifier );
};


#endif