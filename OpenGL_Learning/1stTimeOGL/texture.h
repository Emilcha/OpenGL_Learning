#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <glad/glad.h>


class Texture
{
public:
	int width, height;
	int nrChannels;
	GLuint textureID;
	unsigned char* data;
	Texture(const char* filePath);
	void use(GLuint texUnitIndex);
};

#endif