#include "Skybox.h"

Skybox::Skybox()
	:
	mSkyMesh(nullptr),
	mSkyShader(nullptr),
	mTextureID(0),
	mUniformProjection(0),
	mUniformView(0)
{}

Skybox::Skybox(std::vector<std::string> faceLocations)
{
	// Shader setup
	mSkyShader = new Shader();
	mSkyShader->CreateFromFiles("shaders\\skybox.vert", "shaders\\skybox.frag");

	mUniformProjection = mSkyShader->GetProjectionLocation();
	mUniformView = mSkyShader->GetViewLocation();

	// Texture setup
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	GLint width, height, bitDepth;

	for (size_t i = 0; i < 6; i++)
	{
		unsigned char* textureData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);
		if (!textureData)
		{
			std::cout << "[ERR] Failed to retrieve: " << faceLocations[i].c_str() << std::endl;
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		stbi_image_free(textureData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Mesh setup
	mSkyMesh = new Mesh();
	mSkyMesh->CreateMeshFromFile("meshes\\skybox\\vertices.txt", "meshes\\skybox\\indices.txt", false);
}

Skybox::~Skybox()
{
}

GLvoid Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	glDepthMask(GL_FALSE);

	mSkyShader->UseShader();

	glUniformMatrix4fv(mUniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(mUniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

	mSkyShader->Validate();

	mSkyMesh->RenderMesh();
	
	glDepthMask(GL_TRUE);
}
