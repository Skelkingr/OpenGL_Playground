#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	:
	Light()
{}

DirectionalLight::DirectionalLight(GLfloat r, GLfloat g, GLfloat b, GLfloat ambientIntensity)
	:
	Light(r, g, b, ambientIntensity)
{}

DirectionalLight::~DirectionalLight()
{}
