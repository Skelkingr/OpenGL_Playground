#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "Util.h"

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();	

	virtual GLboolean Init(GLuint width, GLuint height);
	virtual GLvoid Write();
	virtual GLvoid Read(GLenum textureUnit);

	GLuint GetShadowWidth() const { return mShadowWidth; }
	GLuint GetShadowHeight() const { return mShadowHeight; }

protected:
	GLuint mFBO;
	GLuint mShadowMap;

	GLuint mShadowWidth;
	GLuint mShadowHeight;
};

#endif

