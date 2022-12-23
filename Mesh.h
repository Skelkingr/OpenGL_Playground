#ifndef MESH_H
#define MESH_H

#include "util.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat* vertices, GLuint* indices, GLfloat* vertexColors); // unsigned int numOfVertices, unsigned int numOfIndicies);
	void RenderMesh();
	void ClearMesh();

	bool AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	bool CompileShaders();

	const std::vector<GLuint> GetIndices() const { return mIndices; }
	const std::vector<GLfloat> GetVertices() const { return mVertices; }
	const std::vector<GLfloat> GetVertexColors() const { return mVertexColors; }

	GLuint GetVAO() const { return mVAO; }
	GLuint GetVBO1() const { return mVBO1; }
	GLuint GetVBO2() const { return mVBO2; }
	GLuint GetIBO() const { return mIBO; }
	GLuint GetShader() const { return mShader; }

	glm::mat4 GetModel() const { return mModel; }
	glm::mat4 GetProjection() const { return mProjection; }
	GLuint GetUniformModel() const { return mUniformModel; }
	GLuint GetUniformProjection() const { return mUniformProjection; }

	void SetIndices(std::vector<GLuint> indices) { mIndices = indices; }
	void SetVertices(std::vector<GLfloat> vertices) { mVertices = vertices; }
	void SetVertexColors(std::vector<GLfloat> vertexColors) { mVertexColors = vertexColors; }

	void SetVAO(GLuint VAO) { mVAO = VAO; }
	void SetVBO1(GLuint VBO1) { mVBO1 = VBO1; }
	void SetVBO2(GLuint VBO2) { mVBO2 = VBO2; }
	void SetIBO(GLuint IBO) { mIBO = IBO; }
	void SetShader(GLuint shader) { mShader = shader; }

	void SetModel(glm::mat4 model) { mModel = model; }
	void SetProjection(glm::mat4 projection) { mProjection = projection; }
	void SetUniformModel(GLuint uniformModel) { mUniformModel = uniformModel; }
	void SetUniformProjection(GLuint uniformProjection) { mUniformProjection = uniformProjection; }

protected:
	std::vector<GLuint> mIndices;
	std::vector<GLfloat> mVertices;
	std::vector<GLfloat> mVertexColors;

	GLuint mVAO;
	GLuint mVBO1;
	GLuint mVBO2;
	GLuint mIBO;
	GLuint mShader;

	GLsizei mIndexCount;

	glm::mat4 mModel;
	glm::mat4 mProjection;
	GLuint mUniformModel;
	GLuint mUniformProjection;
};

#endif
