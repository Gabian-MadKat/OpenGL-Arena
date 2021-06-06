#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>


#include <iostream>

#include "texture.h"


Texture2D::Texture2D()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}
/*
Texture2D::Texture2D(unsigned int Internal_Format, unsigned int Image_Format, unsigned int Wrap_S, unsigned int Wrap_T, unsigned int Filter_Min, unsigned int Filter_Max)
	: ID(0), Width(0), Height(0), Internal_Format(Internal_Format), Image_Format(Image_Format), Wrap_S(Wrap_S), Wrap_T(Wrap_T), Filter_Min(Filter_Min), Filter_Max(Filter_Max)
{
	glGenTextures(1, &this->ID);
}

Texture2D::Texture2D(unsigned int format)
	: Texture2D(format, format, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST)
{
	glGenTextures(1, &this->ID);
}

Texture2D::~Texture2D()
{
	if (ID)
	{
		glDeleteTextures(1, &ID);
	}
	ID = 0;
}
*/
void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;
	// create Texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	// set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}












/*
Texture2D::Texture2D()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_NEAREST), Filter_Max(GL_NEAREST)
{
	glGenTextures(1, &this->ID);
}

Texture2D::Texture2D(unsigned int Internal_Format, unsigned int Image_Format, unsigned int Wrap_S, unsigned int Wrap_T, unsigned int Filter_Min, unsigned int Filter_Max)
	: ID(0), Width(0), Height(0), Internal_Format(Internal_Format), Image_Format(Image_Format), Wrap_S(Wrap_S), Wrap_T(Wrap_T), Filter_Min(Filter_Min), Filter_Max(Filter_Max)
{}

Texture2D::Texture2D(unsigned int format)
	: Texture2D(format, format, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST)
{}

Texture2D::~Texture2D()
{
	if (ID)
	{
		glDeleteTextures(1, &ID);
	}
	ID = 0;
}

void Texture2D::Load(const char* tex_filename)
{
	int nr_channels;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* image = stbi_load(tex_filename, &this->Width, &this->Height, &nr_channels, STBI_rgb_alpha);

	if (!image)
	{
		std::cerr << "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ: " << tex_filename << std::endl;
		throw;
	}

	Generate(this->Width, this->Height, image);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;
	// bind the texture and set parameters
	glBindTexture(GL_TEXTURE_2D, this->ID);
	// set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	// create the texture image
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
*/