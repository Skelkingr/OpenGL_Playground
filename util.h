#ifndef UTIL_H
#define UTIL_H

/* INCLUDES */ 

#include "stb_image.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* MACROS */
#ifdef _DEBUG
#define GL_CHECK(stmt) \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__);

#else
#define GL_CHECK(stmt) stmt
#endif


/* CONSTANTS */

const GLint MAX_POINT_LIGHTS = 4;
const GLint MAX_SPOT_LIGHTS = 4;

const GLfloat TO_RADIANS = 3.14159265f / 180.0f;


/* HELPER FUNCTIONS */

namespace
{
	GLvoid CheckOpenGLError(const char* stmt, const char* fname, GLint line)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::cout << "OpenGL error " << err << ", at " << fname << ":" << line << " - for " << stmt << std::endl;
			abort();
		}
	}

	GLboolean GetFloatFileContent(std::string fileLocation, std::vector<GLfloat>& vec)
	{
		std::ifstream is(fileLocation.c_str());

		if (!is)
		{
			std::cout << "[ERR] Cannot open file: " << fileLocation << std::endl;
			return false;
		}

		std::string str;
		while (std::getline(is, str))
		{
			if (str.size() <= 0)
				continue;

			size_t pos = 0;
			std::string delimiter = ",";
			
			std::string value;

			while ((pos = str.find(delimiter)) != std::string::npos)
			{
				value = str.substr(0, pos);
				vec.push_back(std::stof(value));
				str.erase(0, pos + delimiter.length());
			}
		}

		return true;
	}

	GLboolean GetIntFileContent(std::string fileLocation, std::vector<GLuint>& vec)
	{
		std::ifstream is(fileLocation.c_str());

		if (!is)
		{
			std::cout << "[ERR] Cannot open file: " << fileLocation << std::endl;
			return false;
		}

		std::string str;
		while (std::getline(is, str))
		{
			if (str.at(0) == '\\' || str.at(0) == '\0')
				continue;

			size_t pos = 0;
			std::string delimiter = ",";

			std::string value;

			while ((pos = str.find(delimiter)) != std::string::npos)
			{
				value = str.substr(0, pos);
				vec.push_back(std::stoi(value));
				str.erase(0, pos + delimiter.length());
			}
		}

		return true;
	}
}

#endif

