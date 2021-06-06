#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.h"
#include "Resource_Manager.h"

#include <iostream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

bool LMB_CLICKED = false;

Game Arena(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arena", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    // ---------------
    Arena.Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    // Mouse Positions
    // ---------------
    double xpos = 0.0f;
    double ypos = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        LMB_CLICKED = false;
        // calculate delta time
        // --------------------
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input
        // -----------------
        glfwGetCursorPos(window, &xpos, &ypos);
        Arena.ProcessInput(deltaTime, xpos, ypos, LMB_CLICKED);

        // update game state
        // -----------------
        Arena.Update(deltaTime);

        // render
        // ------
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Arena.Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Arena.Keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            Arena.Keys[key] = false;
            Arena.KeysProcessed[key] = false;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    glfwGetCursorPos(window, &xpos, &ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        LMB_CLICKED = true;
        double xpos, ypos;
        // get cursor pos
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "Cursor LMB pressed at (" << xpos << " : " << ypos << ")" << std::endl;
    }
}















/*
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include <glm/ext/matrix_clip_space.hpp>

unsigned int WIDTH = 800;
unsigned int HEIGHT = 600;

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return -1;
	}

	// OpenGL ver
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Turn off deprecated functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	Shader* shader = new Shader();
	shader->Compile("resources/shaders/Used/Shader.vs", "resources/shaders/Used/Shader.fs");
	shader->Use();
	shader->setInt("texture1", 0);

	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	shader->setMat4("projection", projection);

	Texture2D* texture = new Texture2D(GL_RGBA);
	texture->Load("resources/textures/Skyrim.png");
	texture->Bind();

	auto src = Rectangle{ 0,0,1,1 };
	auto dst = Rectangle{ 0.0f, 0.0f, 64.0f, 64.0f };
	auto dst2 = Rectangle{ 800.0f - 64.0f, 600.0f - 64.0f, 64.0f, 64.0f };

	
	Renderer* renderer = new Renderer({ 2, 2 });

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		renderer->Draw(src, dst);
		renderer->Draw(src, dst2);

		renderer->Flush();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
*/