#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>	// include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	// the program ID
	unsigned int ID;
	// empty constructor
	Shader() { };
	// constructor reads and builds the shader -- compiling constructor
	// Shader(const char* vertexPath, const char* fragmentPath);
	// sets the current shader as active
	Shader& Use();
	// compiles the shader from given source code
	void Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr); // note: geometry source code is optional
	// use/activate the shader
	// void Use();
	// utility functions
	void    SetFloat(const char* name, float value, bool useShader = false);
	void    SetInteger(const char* name, int value, bool useShader = false);
	void    SetVector2f(const char* name, float x, float y, bool useShader = false);
	void    SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
	void    SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
	void    SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
	void    SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
	void    SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
	void    SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
	// LEGACY
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif