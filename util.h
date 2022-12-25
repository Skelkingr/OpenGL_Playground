#ifndef UTIL_H
#define UTIL_H

#define STB_IMAGE_IMPLEMENTATION 

// INCLUDES
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

// CONSTANTS
const float TO_RADIANS = 3.14159265f / 180.0f;

// TEXT SHADERS
static const char* VSHADER = "									\n\
#version 330													\n\
																\n\
layout (location = 0) in vec3 pos;								\n\
layout (location = 1) in vec3 vertexColors;						\n\
																\n\
out vec3 v_vertexColors;										\n\
																\n\
uniform mat4 model;												\n\
uniform mat4 projection;										\n\
																\n\
void main()														\n\
{																\n\
	gl_Position = projection * model * vec4(pos, 1.0);			\n\
	v_vertexColors = vertexColors;								\n\
}																\n\
";

static const char* FSHADER = "													\n\
#version 330																	\n\
																				\n\
in vec3 v_vertexColors;															\n\
																				\n\
out vec4 color;																	\n\
																				\n\
void main()																		\n\
{																				\n\
	color = vec4(v_vertexColors.r, v_vertexColors.g, v_vertexColors.b, 1.0);	\n\
}																				\n\
";

#endif
