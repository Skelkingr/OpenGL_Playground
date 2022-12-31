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
	void CreateMeshFromFile(const char* vFileLocation, std::vector<GLuint> indices, bool computeNormals);
	void CreateMeshFromFile(const char* vFileLocation, const char* iFileLocation, bool computeNormals);
	void RenderMesh();
	void ClearMesh();

	glm::mat4 GetModel() const { return mModel; }
	glm::mat4 GetProjection() const { return mProjection; }

	bool GetDirection() const { return mDirection; }

	float GetCurrentAngle() const { return mCurrAngle; }

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
