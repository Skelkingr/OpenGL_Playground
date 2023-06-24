#ifndef UTIL_H
#define UTIL_H

/* INCLUDES */ 

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
			printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
			abort();
		}
	}

	GLvoid ComputeAverageNormals(
		std::vector<GLuint>& indices,
		GLuint indicesCount,
		std::vector<GLfloat>& vertices,
		GLuint verticesCount,
		GLuint vLength,
		GLuint normalOffset
	)
	{
		for (size_t i = 0; i < indicesCount; i += 6)
		{
			GLuint in0 = indices[i] * vLength;
			GLuint in1 = indices[i + 1] * vLength;
			GLuint in2 = indices[i + 2] * vLength;
			glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
			glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
			glm::vec3 normal = glm::cross(v1, v2);
			normal = -glm::normalize(normal);

			in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
			vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
			vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
			vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
		}

		for (size_t i = 0; i < verticesCount / vLength; i++)
		{
			GLuint nOffset = i * vLength + normalOffset;
			glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
			vec = glm::normalize(vec);
			vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
		}
	}

	GLboolean GetFloatFileContent(std::string fileLocation, std::vector<GLfloat>& vec)
	{
		std::ifstream is(fileLocation.c_str());

		if (!is)
		{
			printf("[ERR] Cannot open file: %s", fileLocation.c_str());
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
			printf("[ERR] Cannot open file: %s", fileLocation.c_str());
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

