#ifndef MATERIAL_H
#define MATERIAL_H

#include "util.h"

class Material
{
public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shininess);
	~Material();

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

private:
	GLfloat mSpecularIntensity;
	GLfloat mShininess;
};

#endif

