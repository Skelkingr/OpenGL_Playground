#include "AmbientLight.h"

AmbientLight::AmbientLight()
	:
	Light()
{}

AmbientLight::AmbientLight(GLfloat r, GLfloat g, GLfloat b, GLfloat ambientIntensity)
	:
	Light(r, g, b, ambientIntensity)
{}

AmbientLight::~AmbientLight()
{}
