#include "CollisionHandler.h"

void CollisionHandler::init( const Collider& collider )
{
	collider_ = collider;
	relative_ = collider_.velocity;
	collide_state_ = CollideState::NONE;

	deflect_angle_ = vec2( 0.0f, 0.0f );
	surface_normal_ = vec2( 0.0f, 0.0f );
}

void CollisionHandler::setCollider( const Collider& collider )
{
	collider_ = collider;
}

void CollisionHandler::addSurface( const Collider& surface )
{
	surface_cache_.push_back( surface );
}

void CollisionHandler::setSurface( const Collider& surface, GLuint id )
{
	surface_cache_[ id ] = surface;
}

CollideState CollisionHandler::getState() const
{
	return collide_state_;
}

/// <summary>
/// Swept AABB checking
/// </summary>

void CollisionHandler::handleCollision()
{
	size_t i = 0;
	collide_state_ = CollideState::NONE;

	while( ( collide_state_ == CollideState::NONE ) && ( i < surface_cache_.size() ) )
	{
		// Surface collider box
		Collider surface = surface_cache_[ i ];
		relative_ = vec2( collider_.velocity.x - surface.velocity.x, collider_.velocity.y - surface.velocity.y );
		// Check if two boxes have collided using Swept AABB algorithm
		MinkowskiDifference distance = getDistance( surface );
		MinkowskiDifference collision_times = getCollisionTimes( surface, distance );
		time_of_collision_ = getTimeOfCollision( surface, collision_times );

		// Check if there is any collision at all
		if( time_of_collision_ < 1.0f )
		{
			GLfloat x_contact = collider_.velocity.x * time_of_collision_;
			GLfloat y_contact = relative_.y * time_of_collision_;
			surface_normal_ = getSurfaceNormal( collision_times );
			vec2 modifier = collider_.velocity;

			// Paddle Response
			// Set parameters for response calculations
			if( i <= 1 ) // If collision was a paddle
			{
				if( i == 0 ) // If collision was player 1
				{
					collide_state_ = CollideState::DEFLECT;
				}
				else // If collision was player 2
				{
					collide_state_ = CollideState::STICKY;
				}

				if( surface_normal_.y > 0.0f ) // Vertical Collision 
				{
					// collider_.velocity.x = -collider_.velocity.x;
					if( abs( surface.velocity.y ) > abs( collider_.velocity.y ) ) // If paddle y is faster than ball y 
					{
						y_contact = surface.velocity.y - collider_.velocity.y;

						if( collider_.velocity.x > 0.0f ) // Move collider to left or right if vertical hit
							collider_.x = ( surface.x - collider_.w );
						else // Move collider to right if vertical hit
							collider_.x = ( surface.x + surface.w );

						if( abs( relative_.y ) < abs( surface.velocity.y ) ) // If ball or paddle are moving away from each other
						{
							collider_.velocity.y = -collider_.velocity.y;
						}
					}
				}
				collider_.y += y_contact;
				modifier = getHitboxModifier( surface );
			}
			else
			{
				collide_state_ = CollideState::REFLECT;
				collider_.y += y_contact;
			}

			calculateDeflectAngle( modifier );
		}
		i++;
	}
}

glm::vec2 CollisionHandler::getPos() const
{
	return vec2( collider_.x, collider_.y );
}

/// <summary>
/// Find distance between the moving collider and surface x's and y's of the entry and exit sides
/// </summary>
/// <param name="surface"></param>
/// <returns></returns>
MinkowskiDifference CollisionHandler::getDistance( const Collider& surface )
{
	// X and Y distance of entry and exit collision boxes
	vec2 entry;
	vec2 exit;

	// Max X and Y pos of surface and collider
	GLfloat collider_max_x = collider_.x + collider_.w;
	GLfloat collider_max_y = collider_.y + collider_.h;
	GLfloat surface_max_x = surface.x + surface.w;
	GLfloat surface_max_y = surface.y + surface.h;

	// Collider is moving right
	if( relative_.x > 0.0f )
	{
		entry.x = surface.x - collider_max_x;
		exit.x = surface_max_x - collider_.x;
	}
	// Collider is moving left
	else
	{
		entry.x = surface_max_x - collider_.x;
		exit.x = surface.x - collider_max_x;
	}
	
	// Collider is moving down
	if( relative_.y > 0.0f )
	{
		entry.y = surface.y - collider_max_y;
		exit.y = surface_max_y - collider_.y;
	}
	// Collider is moving up
	else
	{
		entry.y = surface_max_y - collider_.y;
		exit.y = surface.y - collider_max_y;
	}

	return { entry, exit };
}

MinkowskiDifference CollisionHandler::getCollisionTimes( const Collider& surface, const MinkowskiDifference& distance ) const
{
	vec2 entry_time;
	vec2 exit_time;

	// Find times of entrance and exits of each axis
	// Time = distance / speed
	entry_time.x = distance.entry.x / relative_.x;
	exit_time.x = distance.exit.x / relative_.x;

	entry_time.y = distance.entry.y / relative_.y;
	exit_time.y = distance.exit.y / relative_.y;

	return { entry_time, exit_time };
	
}
GLfloat	CollisionHandler::getTimeOfCollision( const Collider& surface, const MinkowskiDifference& collision_time ) const
{
	// Find earliest and latest entry and exit times of collision
	// https://gamedev.stackexchange.com/questions/139834/using-max-for-entry-time-in-swept-collision/139855
	vec2 entry_time = vec2( collision_time.entry.x, collision_time.entry.y );
	vec2 exit_time = vec2( collision_time.exit.x, collision_time.exit.y );

	GLfloat latest_entry = std::max( entry_time.x, entry_time.y );
	GLfloat earlist_exit = std::min( exit_time.x, exit_time.y );

	// If there was no collision
	if( latest_entry > earlist_exit )
		return 1.0f;
	if( entry_time.x < 0.0f && entry_time.y < 0.0f )
		return 1.0f;
	if( entry_time.x > 1.0f || entry_time.y > 1.0f )
		return 1.0f;

	// Check if x position overlaps 
	if( entry_time.x < 0.0f )
	{
		if( ( collider_.x + collider_.w ) < surface.x || collider_.x > ( surface.x + surface.w ) )
			return 1.0f;
	}
	// Check if y position overlaps
	if( entry_time.y < 0.0f )
	{
		if( ( collider_.y + collider_.h ) < surface.y || collider_.y > ( surface.y + surface.h ) )
			return 1.0f;
	}

	return latest_entry;
}

glm::vec2 CollisionHandler::getSurfaceNormal( const MinkowskiDifference& collision_times ) const
{
	vec2 surface_normal;

	if( collision_times.entry.x > collision_times.entry.y )
	{
		surface_normal = vec2( 1.0f, 0.0f );
	}
	else
	{
		surface_normal = vec2( 0.0f, 1.0f );
	}
	
	return surface_normal;
}

glm::vec2 CollisionHandler::getHitboxModifier( const Collider& paddle ) const
{
	GLfloat paddle_mid = paddle.y + ( paddle.h  / 2 );
	GLfloat collider_mid = collider_.y + ( collider_.h / 2 );

	vec2 collider_velocity = collider_.velocity;
	GLfloat x_modified, y_modified;

	// Paddle Hitboxes
	GLfloat upper_quad, lower_quad;
	GLfloat upper_mid, lower_mid;

	upper_quad = paddle.y + ( paddle.h * ( 2 / paddle.h ) );
	upper_mid = paddle_mid - ( ( paddle.h  * 0.25f ) / 2 );
	lower_quad = ( paddle.y + paddle.h ) - ( paddle.h * ( 2 / paddle.h ) );
	lower_mid = paddle_mid + ( ( paddle.h  * 0.25f ) / 2 );

	if( ( collider_mid < upper_quad ) || ( collider_mid > lower_quad ) ) // Hits top or bottom of paddle ( -15% X, +50% Y )
	{
		x_modified = collider_velocity.x * 0.75f;
		y_modified = collider_velocity.y * 2.0f;
	}
	else if( ( ( collider_mid >= upper_quad ) && ( collider_mid < upper_mid ) ) || ( ( collider_mid > lower_mid ) && ( collider_mid <= lower_quad ) ) ) // Hits upper quadrant or lower quadrant of paddle ( +25% X, +25% Y )
	{
		x_modified = collider_velocity.x * 1.25f;
		y_modified = collider_velocity.y;
		GLfloat abs_y = abs( collider_.velocity.y );

		if( abs( collider_.velocity.y ) < 0.1f )
		{
			if( collider_.velocity.y < 0.0f )
				y_modified = -( abs( collider_.x ) );
			else
				y_modified = abs( collider_.x );
		}
			
	}
	else // Hits middle top or middle bottom (+50% X, -15% Y )
	{
		x_modified = collider_velocity.x * 2.0f;
		y_modified = collider_velocity.y * 0.5f;
	}

	return vec2( x_modified, y_modified );
}

void CollisionHandler::calculateDeflectAngle( const vec2& modifier )
{

	deflect_angle_.x = modifier.x;
	deflect_angle_.y = modifier.y;

	switch( collide_state_ )
	{
		case( CollideState::DEFLECT ): // Player 1 Reflect property
			deflect_angle_.x = abs( deflect_angle_.x );
			break;
		case( CollideState::STICKY ): // Player 2 Reflect Property
			deflect_angle_.x = -abs( deflect_angle_.x );
			break;
		default: // Wall or Moving
			break;
	}
	
	if( surface_normal_.y > 0.0f )
		deflect_angle_.y = -deflect_angle_.y;
}

glm::vec2 CollisionHandler::getDeflectAngle() const
{
	return deflect_angle_;
}

CollisionHandler::CollisionHandler() {}
CollisionHandler::~CollisionHandler() {}