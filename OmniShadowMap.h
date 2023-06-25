#ifndef OMNISHADOWMAP_H
#define OMNISHADOWMAP_H

#include "ShadowMap.h"

class OmniShadowMap : ShadowMap
{
public:
	OmniShadowMap();
	~OmniShadowMap();

	GLboolean Init(GLuint width, GLuint height);
	GLvoid Write();
	GLvoid Read(GLenum textureUnit);
};

#endif
