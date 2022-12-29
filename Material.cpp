#include "Material.h"

Material::Material()
	:
	mSpecularIntensity(0.0f),
	mShininess(0.0f)
{}

Material::Material(GLfloat specularIntensity, GLfloat shininess)
{
	mSpecularIntensity = specularIntensity;
	mShininess = shininess;
}

Material::~Material()
{}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, mSpecularIntensity);
	glUniform1f(shininessLocation, mShininess);
}

