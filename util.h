#ifndef UTIL_H
#define UTIL_H

#include <cmath>

#include <stdio.h>
#include <string.h>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const char* VSHADER = "												\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec3 pos;											\n\
																			\n\
uniform float xMove;														\n\
																			\n\
void main()																	\n\
{																			\n\
	gl_Position = vec4(0.4 * pos.x + xMove, 0.4 * pos.y, pos.z, 1.0);		\n\
}																			\n\
";

static const char* FSHADER = "									\n\
#version 330													\n\
																\n\
out vec4 color;													\n\
																\n\
void main()														\n\
{																\n\
	color = vec4(0.0, 1.0, 0.5, 1.0);						 	\n\
}																\n\
";

#endif
