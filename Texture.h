#ifndef TEXTURE_H
#define TEXTURE_H

#include "Util.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLocation); 
	~Texture();

	GLboolean LoadTexture();
	GLboolean LoadTextureA();

	GLvoid UseTexture();
	GLvoid ClearTexture();
	GLvoid ResetTexture();

private:
	GLuint mTextureID;
	GLint mWidth;
	GLint mHeight;
	GLint mBitDepth;

	const char* mFileLocation;
};

#endif



