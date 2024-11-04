#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Material
{
public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shininess);
	~Material();

	void UseMaterial(GLint specularIntensityLocation, GLint shininessLocation) const;
private:
	GLfloat specularIntensity;
	GLfloat shininess;

};

