#ifndef MESH_H
#define MESH_H

#include "util.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat* vertices, GLuint* indices, GLfloat* vertexColors, GLuint numOfVertices, GLuint numOfIndices, GLuint numOfVertexColors);
	void RenderMesh();
	void ClearMesh();

	GLuint GetVAO() const { return mVAO; }
	GLuint GetVBO1() const { return mVBO1; }
	GLuint GetVBO2() const { return mVBO2; }
	GLuint GetIBO() const { return mIBO; }

	GLsizei GetIndexCount() const { return mIndexCount; }

	glm::mat4 GetModel() const { return mModel; }
	glm::mat4 GetProjection() const { return mProjection; }

	void SetVAO(GLuint VAO) { mVAO = VAO; }
	void SetVBO1(GLuint VBO1) { mVBO1 = VBO1; }
	void SetVBO2(GLuint VBO2) { mVBO2 = VBO2; }
	void SetIBO(GLuint IBO) { mIBO = IBO; }

	void SetModel(glm::mat4 model) { mModel = model; }
	void SetProjection(glm::mat4 projection) { mProjection = projection; }

protected:
	GLuint mVAO;
	GLuint mVBO1;
	GLuint mVBO2;
	GLuint mIBO;

	GLsizei mIndexCount;

	glm::mat4 mModel;
	glm::mat4 mProjection;
};

#endif
