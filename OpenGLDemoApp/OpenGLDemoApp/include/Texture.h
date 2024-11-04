#pragma once


#include <GL/glew.h>
#include <stb_image.h>


class Texture
{

public:
	Texture(const char *fileLoc);
	Texture();
	~Texture();

	bool LoadTexture();
	bool LoadTextureWithAlpha();
	void UseTextureOnTextureUnit(GLenum textureUnit);
	void ClearTexture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;


};

