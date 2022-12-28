#ifndef MESH_H
#define MESH_H

#include "Util.h"

class Mesh
{
public:
	Mesh();
	Mesh(bool direction, float offset, float maxOffset, float increment);
	~Mesh();
		
	void CreateMesh(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, GLuint numOfVertices, GLuint numOfIndices);
	void RenderMesh();
	void ClearMesh();

	GLuint GetVAO() const { return mVAO; }
	GLuint GetVBO() const { return mVBO; }
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
	void SetVBO(GLuint VBO) { mVBO = VBO; }
	void SetIBO(GLuint IBO) { mIBO = IBO; }

	void SetModel(glm::mat4 model) { mModel = model; }
	void SetProjection(glm::mat4 projection) { mProjection = projection; }

	void SetDirection(bool direction) { mDirection = direction; }
	void SetOffset(float offset) { mOffset = offset; }

	void SetCurrentAngle(float angle) { mCurrAngle = angle; }

protected:
	GLuint mVAO;
	GLuint mVBO;
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
