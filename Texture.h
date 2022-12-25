#ifndef TEXTURE_H
#define TEXTURE_H

#include "Util.h"
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLocation); 
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();
	void ResetTexture();

private:
	GLuint mTextureID;
	int mWidth;
	int mHeight;
	int mBitDepth;

	char* mFileLocation;
};

#endif



