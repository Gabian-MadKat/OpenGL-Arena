#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Shaders
const char* vertexShaderSrc = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"		// the position variable has attribute position 0
	"layout (location = 1) in vec3 aColor;\n"	// the color variable has attribute position 1
	"\n"
	"out vec3 ourColor;\n"		// output a color to the fragment shader
	"\n"
	"uniform vec4 offset"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x + offset.x	, aPos.y, aPos.z, 1.0);\n"
	"	ourColor = aColor;\n"	// set ourColor to the input color we got from the vertex data
	"}\0";

const char* fragmentShaderSrc = "#version 330 core\n"
	"out vec4 FragColor;\n"
//	"\n"
//	"uniform vec4 ourColor;\n"	// uniform vec4 value to pass data from CPU to GPU
	"in vec3 ourColor;\n"
	"\n"
	"void main()\n"
	"{\n"
//	"	FragColor = ourColor;\n"	// from a static vec4(1.0f, 0.5f, 0.2f, 1.0f) to a dynamically modified color variable in time
	"	FragColor = vec4(ourColor, 1.0);\n"
	"}\n\0";

int main()
{
	// Init and config
	glfwInit();
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac-Only essential

	// Window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangles and Rectangles", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load OpenGL functions and pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile shader program
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
	// check for compile error
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);
	// check for compile error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// delete unnecessary shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Let's draw a triangle! We will use a VBO!
	//-------------------------------------------
	/*
	// set up vertex data and (and buffer/buffers) and configure vertex attributes
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// create a vertex buffer object (VBO) and generate buffers with the reference ID 1
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// bind the vertex buffer object (VBO) to the vertex buffer array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Stream draw - draws once, stays a little time
	// Static draw - draws once, stays a long time
	// Dynamic draw - draws (updates) multiple times, stays for a long time

	// Linking vertex attributes (set vertex attributes pointers)
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// glEnableVertexAttribArray(0);	// enable vertex attribute
	
	// Create a VAO (Vertex Array Object)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO); // just like a VBO :)

	// Initialization code (done once - unless object frequently changes)
	// Bind VAO
	glBindVertexArray(VAO);
	// Copy vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Drawing code (render loop)
	// Draw the object
	glUseProgram(shaderProgram);
	glBindVertexArray(0);
	//function
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	*/

	// Let's draw a RECTANGLE!! We will use an EBO (Element Buffer Object)
	//-----------------------------------------------------------------------

	/*
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,		// top right
		 0.5f, -0.5f, 0.0f,		// bottom right
		-0.5f, -0.5f, 0.0f,		// bottom left
		-0.5f,  0.5f, 0.0f		// top left
	};
	unsigned int indices[] = {		// start from 0
		0, 1, 3,	// first triangle
		1, 2, 3		// second triangle
	};
	*/

	float vertices[] = {
		// positions xyz	// colors rgb
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	// bottom left
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f	// top
	};

	unsigned int VBO, VAO;
	// unsigned int EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	// glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// copy vertices to VBO for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// copy index array in EBO for OpenGL to use
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(VAO);

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate the shader
		glUseProgram(shaderProgram);

		// draw triangle
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");	// gets the uniform ourColor
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);		// sets the value for the uniform ourColor

		// render triangle
		// glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// draw wireframe to see that indeed there are 2 triangles being drawn
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		// draw in wireframe (LINE) mode. To draw in fill mode use GL_FILL instead of GL_LINE
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	// draw a rectangle out of 6 verticles in total (2 triangles each with 3 verticles)
		// glBindVertexArray(0);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources that are now unused
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// terminate GLFW program
	glfwTerminate();

	// Get maximum number of vertex attributes
	/*
	long long int vertexAttributesCount;
	glGetInteger64v(GL_MAX_VERTEX_ATTRIBS, &vertexAttributesCount);
	std::cout << "Maximum number of vertex attributes supported by the shader language (GLSL): " << vertexAttributesCount << std::endl;
	*/

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// Execute on windows size change
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Viewport will have the new window dimensions
	glViewport(0, 0, width, height);
}