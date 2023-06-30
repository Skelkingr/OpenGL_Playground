#include "Mesh.h"

Mesh::Mesh()
	:
	mVAO(0),
	mVBO(0),
	mIBO(0),
	mIndexCount(0),
	mModel(NULL),
	mProjection(NULL),
	mDirection(true),
	mOffset(0.0f),
	mMaxOffset(1.0f),
	mIncrement(0.0005f),
	mCurrAngle(0.0f)
{}

Mesh::Mesh(bool direction, float offset, float maxOffset, float increment)
	:
	mVAO(0),
	mVBO(0),
	mIBO(0),
	mIndexCount(0),
	mModel(NULL),
	mProjection(NULL),
	mCurrAngle(0.0f)
{
	mDirection = direction;
	mOffset = offset;
	mMaxOffset = maxOffset;
	mIncrement = increment;
}

Mesh::~Mesh()
{
	ClearMesh();
}

GLvoid Mesh::ComputeAverageNormals(
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

void Mesh::CreateMesh(GLfloat* vertices, GLuint* indices, GLuint numOfVertices, GLuint numOfIndices)
{
	mIndexCount = numOfIndices;

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::CreateMeshFromFile(const char* vFileLocation, std::vector<GLuint> indices, bool computeNormals)
{
	std::vector<GLfloat> vertices;

	bool vResult = GetFloatFileContent(vFileLocation, vertices);

	if (!vResult)
	{
		std::cout << "[ERR] Could not create mesh from vertices file: " << vFileLocation << std::endl;
		return;
	}

	if (computeNormals)
		ComputeAverageNormals(indices, indices.size(), vertices, vertices.size() / 8, 8, 5);

	CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
}

void Mesh::CreateMeshFromFile(const char* vFileLocation, const char* iFileLocation, bool computeNormals)
{
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	bool vResult = GetFloatFileContent(vFileLocation, vertices);
	bool iResult = GetIntFileContent(iFileLocation, indices);

	if (!vResult)
	{
		std::cout << "[ERR] Could not create mesh from vertices file: " << vFileLocation << std::endl;
		return;
	}
	if (!iResult)
	{
		std::cout << "[ERR] Could not create mesh from indices file: " << iFileLocation << std::endl;
		return;
	}

	if (computeNormals)
		ComputeAverageNormals(indices, indices.size(), vertices, vertices.size() / 8, 8, 5);

	CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
}

void Mesh::RenderMesh()
{
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (mIBO != 0)
	{
		glDeleteBuffers(1, &mIBO);
		mIBO = 0;
	}

	if (mVBO != 0)
	{
		glDeleteBuffers(1, &mVBO);
		mVBO = 0;
	}

	if (mVAO != 0)
	{
		glDeleteVertexArrays(1, &mVAO);
		mVAO = 0;
	}

	mIndexCount = 0;
}
