#include "Mesh.h"

Mesh::Mesh()
	:
	mVBO1(0),
	mVBO2(0),
	mIBO(0),
	mShader(0),
	mIndexCount(0),
	mModel(NULL),
	mProjection(NULL),
	mUniformModel(0),
	mUniformProjection(0)
{
	mVAO = 0;
}

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

bool Mesh::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1] = { 0 };
	theCode[0] = shaderCode;

	GLint codeLength[1] = { 0 };
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(theShader, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error compiling the %d shader: '%s' \n", shaderType, eLog);
		return false;
	}

	glAttachShader(theProgram, theShader);

	return true;
}

bool Mesh::CompileShaders()
{
	mShader = glCreateProgram();

	if (!mShader)
	{
		printf("[ERR] Error creating shader program.");
		return false;
	}

	AddShader(mShader, VSHADER, GL_VERTEX_SHADER);
	AddShader(mShader, FSHADER, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(mShader);
	glGetProgramiv(mShader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(mShader, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error linking program: '%s' \n", eLog);
		return false;
	}

	glValidateProgram(mShader);
	glGetProgramiv(mShader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(mShader, sizeof(eLog), nullptr, eLog);
		printf("[ERR] Error validating program: '%s' \n", eLog);
		return false;
	}

	mUniformModel = glGetUniformLocation(mShader, "model");
	mUniformProjection = glGetUniformLocation(mShader, "projection");

	return true;
}
