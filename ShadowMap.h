#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include "Util.h"

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);

	GLuint GetShadowWidth() const { return mShadowWidth; }
	GLuint GetShadowHeight() const { return mShadowHeight; }

protected:
	GLuint mFBO;
	GLuint mShadowMap;

	GLuint mShadowWidth;
	GLuint mShadowHeight;
};

#endif

