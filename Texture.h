#ifndef TEXTURE_H
#define TEXTURE_H

#include "Util.h"
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation); 
	~Texture();

	bool LoadTexture();
	bool LoadTextureA();

	void UseTexture();
	void ClearTexture();
	void ResetTexture();

private:
	GLuint mTextureID;
	int mWidth;
	int mHeight;
	int mBitDepth;

	const char* mFileLocation;
};

#endif



