#include "Projectile.h"

ProjectileObject::ProjectileObject()
	: GameObject(), Radius(5), Stuck(true), Destroy(false)
{
}

ProjectileObject::ProjectileObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true), Destroy(false)
{
}

ProjectileObject::~ProjectileObject()
{
	this->Destroy = true;
	// std::cout << "Should be deleted." << std::endl;
}

glm::vec2 ProjectileObject::Move(float dt, unsigned int window_width, unsigned int window_height)
{
	// if not stuck to the player
	if (!this->Stuck)
	{
		// move projectile
		this->Position += this->Velocity * dt;
		// check if outside window bounds; if so, delete the projectile
		
	}
	return this->Position;
}

void ProjectileObject::CheckIfCollided(unsigned int window_width, unsigned int window_height)
{
	if (this->Position.x <= 0.0f)
	{
		this->Destroy = true;
	}
	else if (this->Position.x + this->Size.x >= window_width)
	{
		this->Destroy = true;
	}
	if (this->Position.y <= 0.0f)
	{
		this->Destroy = true;
	}
	else if (this->Position.y + this->Size.y >= window_height)
	{
		this->Destroy = true;
	}
}

void ProjectileObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->Position = position;
	this->Velocity = velocity;
	this->Stuck = true;
}
