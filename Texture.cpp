#include "Texture.h"

Texture::Texture()
	:
	mTextureID(0),
	mWidth(0),
	mHeight(0),
	mBitDepth(0),
	mFileLocation((char*)"")
{}

Texture::Texture(char* fileLocation)
	:
	mTextureID(0),
	mWidth(0),
	mHeight(0),
	mBitDepth(0)
{
	mFileLocation = (char*)fileLocation;
}

Texture::~Texture()
{
	ClearTexture();
}

bool Texture::LoadTexture()
{
	unsigned char* textureData = stbi_load(mFileLocation, &mWidth, &mHeight, &mBitDepth, 0);
	if (!textureData)
	{
		printf("[ERR] Failed to retrieve: %s\n", mFileLocation);
		return false;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);

	return true;
}

bool Texture::LoadTextureA()
{
	unsigned char* textureData = stbi_load(mFileLocation, &mWidth, &mHeight, &mBitDepth, 0);
	if (!textureData)
	{
		printf("[ERR] Failed to retrieve: %s\n", mFileLocation);
		return false;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);

	return true;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &mTextureID);
	ResetTexture();
}

void Texture::ResetTexture()
{
	mTextureID = 0;
	mWidth = 0;
	mHeight = 0;
	mBitDepth = 0;
	mFileLocation = (char*)"";
}
