#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "Particle_Generator.h"

class ProjectileObject : public GameObject
{
public:
	// projectile state
	float Radius;
	bool Stuck;
	bool Destroy;

	ProjectileObject();
	ProjectileObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
	~ProjectileObject();

	glm::vec2 Move(float dt, unsigned int window_width, unsigned int window_height);
	void Reset(glm::vec2 position, glm::vec2 velocity);
	void CheckIfCollided(unsigned int window_width, unsigned int window_height);
};