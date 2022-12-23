#include "Mesh.h"

Mesh::Mesh()
	:
	mVAO(0),
	mVBO1(0),
	mVBO2(0),
	mIBO(0),
	mIndexCount(0),
	mModel(NULL),
	mProjection(NULL)
{}

Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::CreateMesh(GLfloat* vertices, GLuint* indices, GLfloat* vertexColors, GLuint numOfVertices, GLuint numOfIndices, GLuint numOfVertexColors)
{
	mIndexCount = numOfIndices;

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	// Index buffer
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	// Vertex buffer
	glGenBuffers(1, &mVBO1);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Vertex color buffer
	glGenBuffers(1, &mVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors[0]) * numOfVertexColors, vertexColors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// Unbind
	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	if (mVBO2 != 0)
	{
		glDeleteBuffers(1, &mVBO2);
		mVBO2 = 0;
	}

	if (mVBO1 != 0)
	{
		glDeleteBuffers(1, &mVBO1);
		mVBO1 = 0;
	}

	if (mVAO != 0)
	{
		glDeleteVertexArrays(1, &mVAO);
			mVAO = 0;
	}

	mIndexCount = 0;
}
