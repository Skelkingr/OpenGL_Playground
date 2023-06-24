#include "ShadowMap.h"

ShadowMap::ShadowMap()
	:
	mFBO(0),
	mShadowMap(0),
	mShadowWidth(0),
	mShadowHeight(0)
{}

ShadowMap::~ShadowMap()
{
	if (mFBO)
	{
		glDeleteFramebuffers(1, &mFBO);
	}

	if (mShadowMap)
	{
		glDeleteTextures(1, &mShadowMap);
	}
}

GLboolean ShadowMap::Init(GLfloat width, GLfloat height)
{
	mShadowWidth = width;
	mShadowHeight = height;

	glGenFramebuffers(1, &mFBO);

	glGenTextures(1, &mShadowMap);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowWidth, mShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("[ERR] Framebuffer error: %i\n", status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

GLvoid ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

GLvoid ShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, mShadowMap);
}
