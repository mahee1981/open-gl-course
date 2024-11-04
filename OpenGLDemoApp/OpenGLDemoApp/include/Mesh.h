#pragma once

#include <GL/glew.h>
#include <vector>

class Mesh
{
public:
	Mesh();
	Mesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	Mesh(const std::vector<GLfloat>& vertices, const std::vector<unsigned int>& indices);


	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void CreateMesh(const std::vector<GLfloat> &vertices, const std::vector<unsigned int> &indices);
	void RenderMesh() const;
	void ClearMesh();

	~Mesh();

private:
	GLuint vertexArrayObject, vertexBufferObject, elementBufferObject;
	GLsizei indexCount;
};

