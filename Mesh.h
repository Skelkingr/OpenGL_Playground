#ifndef MESH_H
#define MESH_H

#include "util.h"

class Mesh
{
public:
	Mesh();
	Mesh(bool direction, float offset, float maxOffset, float increment);
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

	bool GetDirection() const { return mDirection; }
	float GetOffset() const { return mOffset; }
	float GetMaxOffset() const { return mMaxOffset; }
	float GetIncrement() const { return mIncrement; }

	float GetCurrentAngle() const { return mCurrAngle; }

	void SetVAO(GLuint VAO) { mVAO = VAO; }
	void SetVBO1(GLuint VBO1) { mVBO1 = VBO1; }
	void SetVBO2(GLuint VBO2) { mVBO2 = VBO2; }
	void SetIBO(GLuint IBO) { mIBO = IBO; }

	void SetModel(glm::mat4 model) { mModel = model; }
	void SetProjection(glm::mat4 projection) { mProjection = projection; }

	void SetDirection(bool direction) { mDirection = direction; }
	void SetOffset(float offset) { mOffset = offset; }

	void SetCurrentAngle(float angle) { mCurrAngle = angle; }

protected:
	GLuint mVAO;
	GLuint mVBO1;
	GLuint mVBO2;
	GLuint mIBO;

	GLsizei mIndexCount;

	glm::mat4 mModel;
	glm::mat4 mProjection;

	bool mDirection;
	float mOffset;
	float mMaxOffset;
	float mIncrement;

	float mCurrAngle;
};

#endif
