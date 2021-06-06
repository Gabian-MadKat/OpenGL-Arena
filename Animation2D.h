#pragma once

#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <vector>

enum AnimationType
{
	Idle,
	Walk,
	Run,
	Attack,
	Block,
	Cast
};

class Animation2D
{
public:
	// c-tor
	Animation2D(const char* filename);
	// d-tor
	~Animation2D();
	// Play the animation
	void Play(Texture2D& spritesheet, Shader& shader, float dt);
	// Sets the animation speed
	void SetAnimationSpeed(float newspeed);
private:
	double anim_cursor;
	int current_frame_index;
	int frame_count;
	float speed;

	std::vector<glm::vec4> frames;
};