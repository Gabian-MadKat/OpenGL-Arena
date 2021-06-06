#pragma once
#include <glad/glad.h>
#include <vector>
#include "Rectangle.h"

class Renderer
{
	unsigned int VBO, VAO, Attrib_size;
	std::vector<float> buffer;
	void Init(std::vector<unsigned int> attributes);
public:
	Renderer(std::vector<unsigned int> attributes)
	{
		Init(attributes);
	}

	~Renderer()
	{
		if (VBO)
		{
			glDeleteBuffers(1, &VBO);
		}

		if (VAO)
		{
			glDeleteBuffers(1, &VAO);
		}
	}

	void Draw(Rectangle& src, Rectangle& dest);
	void Flush();
};