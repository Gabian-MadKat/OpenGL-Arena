#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

// Texture 2D is able to store and configure a texture in OpenGL
// It also hosts utility functions for easy management
class Texture2D
{
public:
	// holds the ID of the texture object, used for all texture operations to reference to this particular texture
	unsigned int ID;
	// texture image dimensions
	int Width, Height; // width and height of loaded image in pixels
	// texture Format
	unsigned int Internal_Format; // format of texture object
	unsigned int Image_Format; // format of loaded image
	// texture configuration
	unsigned int Wrap_S; // wrapping mode on S axis
	unsigned int Wrap_T; // wrapping mode on T axis
	unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
	unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
	// c-tor (sets default texture modes)
	Texture2D();
	//Texture2D(unsigned int Internal_Format, unsigned int Image_Format, unsigned int Wrap_S, unsigned int Wrap_T, unsigned int Filter_Min, unsigned int Filter_Max);
	//Texture2D(unsigned int format);
	//~Texture2D();
	// load the texture
	//void Load(const char* tex_filename);
	// generates texture from image data
	void Generate(unsigned int width, unsigned int height, unsigned char* data);
	// binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};

#endif