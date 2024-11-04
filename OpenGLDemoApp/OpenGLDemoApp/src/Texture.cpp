#include "..\include\Texture.h"
#include <iostream>

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	this->bitDepth = 0;
	this->fileLocation = fileLoc;
	this->height = 0;
	
}

Texture::Texture()
{
	textureID = 0;
	width = 0;
	this->bitDepth = 0;
	this->fileLocation = "";
	this->height = 0;
}

Texture::~Texture()
{
	ClearTexture();
}

bool Texture::LoadTextureWithAlpha()
{

	// generate texture Object and bind to target
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set texture wrapping and what happens when the texture is scaled up or down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // we don't use mipmaps when upscaling the image, makes no sense
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* texData = stbi_load(this->fileLocation, &width, &height, &bitDepth, STBI_rgb_alpha);
	if (!texData)
	{
		std::cout << "Failed to find " << fileLocation << std::endl;
		stbi_image_free(texData);
		return false;
	}
	// load texture image into the texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;

}

bool Texture::LoadTexture()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set texture wrapping and what happens when the texture is scaled up or down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // we don't use mipmaps when upscaling the image, makes no sense
	stbi_set_flip_vertically_on_load(true);
	unsigned char* texData = stbi_load(this->fileLocation, &width, &height, &bitDepth, STBI_rgb_alpha);
	if (!texData)
	{
		std::cout << "Failed to find " << fileLocation << std::endl;
		stbi_image_free(texData);
		return false;
	}
	// load texture image into the texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;
}

void Texture::UseTextureOnTextureUnit(GLenum textureUnit)
{
	//make a texture unit active
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

void Texture::ClearTexture()
{
	std::cout << "Texture destroyed" << std::endl;
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	this->bitDepth = 0;
	this->fileLocation = "";
	this->height = 0;
}
