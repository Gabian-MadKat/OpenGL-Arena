#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

class SpriteRenderer
{
public:
	// C-tor (inits shaders/shapes)
	SpriteRenderer(Shader& shader);
	// D-tor
	~SpriteRenderer();
	// Renders a defined quad textured with given sprite
	void DrawSprite(Texture2D & texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
private:
	// Render state
	Shader shader;
	unsigned int quadVAO;
	// Initializes and config the quad's buffer and vertex attribs
	void initRenderData();
};