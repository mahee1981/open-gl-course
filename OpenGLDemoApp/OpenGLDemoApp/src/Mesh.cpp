#include "..\include\Mesh.h"
#include <iostream>

Mesh::Mesh() 
{
	vertexArrayObject = 0;
	vertexBufferObject = 0;
	elementBufferObject = 0;
	indexCount = 0;
}

Mesh::Mesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	vertexArrayObject = 0;
	vertexBufferObject = 0;
	elementBufferObject = 0;
	indexCount = 0;
	CreateMesh(vertices, indices, numOfVertices, numOfIndices);
}

Mesh::Mesh(const std::vector<GLfloat>& vertices, const std::vector<unsigned int>& indices)
{
	vertexArrayObject = 0;
	vertexBufferObject = 0;
	elementBufferObject = 0;
	indexCount = 0;
	CreateMesh(vertices, indices);
}


void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

	// generate and bind the objects vertex array (what each of the attributes means)
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Indexes of individual vertices
	glGenBuffers(1, &elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indices, GL_STATIC_DRAW);

	// vertex data
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfVertices, vertices, GL_STATIC_DRAW);

	// which attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3) );
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5) );
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::CreateMesh(const std::vector<GLfloat>& vertices, const std::vector<unsigned int>& indices)
{
	indexCount = indices.size();

	// generate and bind the objects vertex array (what each of the attributes means)
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Indexes of individual vertices
	glGenBuffers(1, &elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indices.data(), GL_STATIC_DRAW);

	// vertex data
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// which attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh() const
{
	// bind VAO and EBO
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	
	// issue draw command
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	// unbind VAO and EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Mesh::ClearMesh()
{
	if (elementBufferObject != 0)
	{
		glDeleteBuffers(1, &elementBufferObject);
		elementBufferObject = 0;
	}
	if (vertexArrayObject != 0)
	{
		glDeleteVertexArrays(1, &vertexArrayObject);
		vertexArrayObject = 0;
	}
	if (vertexBufferObject != 0)
	{
		glDeleteBuffers(1, &vertexBufferObject);
		vertexBufferObject = 0;
	}
	indexCount = 0;
}


Mesh::~Mesh() 
{
	std::cout << "Mesh destroyed" << std::endl;

	ClearMesh();
}