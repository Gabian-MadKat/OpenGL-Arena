#include "Renderer.h"

void Renderer::Init(std::vector<unsigned int> attributes)
{
	// calculate total attribute size
	Attrib_size = 0;
	for (auto att : attributes)
	{
		Attrib_size += att;
	}

	// create storage in GPU
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	auto stride = 0ull;
	for (auto i = 0u; i < attributes.size(); ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, Attrib_size * sizeof(float), (void*)stride);
		stride += attributes[i] * sizeof(float);
	}
}

void Renderer::Draw(Rectangle& src, Rectangle& dest)
{
	// bot left
	buffer.push_back(dest.x);
	buffer.push_back(dest.y + dest.h);
	buffer.push_back(src.x);
	buffer.push_back(src.y + src.h);

	// top right
	buffer.push_back(dest.x + dest.w);
	buffer.push_back(dest.y);
	buffer.push_back(src.x + src.w);
	buffer.push_back(src.y);

	// top left
	buffer.push_back(dest.x);
	buffer.push_back(dest.y);
	buffer.push_back(src.x);
	buffer.push_back(src.y);

	// bot left
	buffer.push_back(dest.x);
	buffer.push_back(dest.y + dest.h);
	buffer.push_back(src.x);
	buffer.push_back(src.y + src.h);

	// bot right
	buffer.push_back(dest.x + dest.w);
	buffer.push_back(dest.y + dest.h);
	buffer.push_back(src.x + src.w);
	buffer.push_back(src.y + src.h);

	// top right
	buffer.push_back(dest.x + dest.w);
	buffer.push_back(dest.y);
	buffer.push_back(src.x + src.w);
	buffer.push_back(src.y);
}

void Renderer::Flush()
{
	// edit changes to the dynamic buffer in OpenGL
	glBufferData(GL_ARRAY_BUFFER, this->buffer.size() * sizeof(float), this->buffer.data(), GL_STATIC_DRAW);

	// draw rectangles
	glDrawArrays(GL_TRIANGLES, 0, this->buffer.size() / this->Attrib_size);

	this->buffer.clear();
}
