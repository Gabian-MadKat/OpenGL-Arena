#include "Animation2D.h"

#include <string>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

Animation2D::Animation2D(const char* filename)
	: anim_cursor(0), current_frame_index(0), speed(0.05f)
{
	FILE* fp = nullptr;
	const int bufferlen = 255;
	char line[bufferlen];

	fopen_s(&fp, filename, "r");

	if (fp == nullptr)
	{
		std::cout << "Error reading animation file." << std::endl;
	}
	else
	{
		while (fgets(line, bufferlen, fp))
		{
			std::vector<int> result;
			std::stringstream ss(line);
			std::string token;
			while (std::getline(ss, token, ','))
			{
				result.push_back(stoi(token));
			}

			glm::vec4 frame = glm::vec4(result[0], result[1], result[2], result[3]);
			frames.push_back(frame);
		}
		fclose(fp);
	}

	frame_count = (int)frames.size()
}

Animation2D::~Animation2D()
{
}

void Animation2D::Play(Texture2D& spritesheet, Shader& shader, float dt)
{
	anim_cursor += dt;

	if (anim_cursor > speed)
	{
		current_frame_index = (current_frame_index + 1) % frame_count;
		anim_cursor = 0;
	}

	glm::vec4 frame = frames[current_frame_index];

	// Normalization
	frame.x /= spritesheet.Width;
	frame.y /= spritesheet.Height;
	frame.z /= spritesheet.Width;
	frame.w /= spritesheet.Height;

	std::vector<glm::vec2> uv;

	uv = {
		glm::vec2(frame.x, frame.y),
		glm::vec2(frame.x, frame.y + frame.w),
		glm::vec2(frame.x + frame.z, frame.y),

		glm::vec2(frame.x + frame.z, frame.y),
		glm::vec2(frame.x, frame.y + frame.w),
		glm::vec2(frame.x + frame.z, frame.y + frame.w)
	};

	glBindVertexArray()
}

void Animation2D::SetAnimationSpeed(float newspeed)
{
}
