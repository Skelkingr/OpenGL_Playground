#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Util.h"

class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations);
	~Skybox();

	GLvoid DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
	Mesh* mSkyMesh;
	Shader* mSkyShader;

	GLuint mTextureID;
	GLuint mUniformProjection;
	GLuint mUniformView;
};

#endif
