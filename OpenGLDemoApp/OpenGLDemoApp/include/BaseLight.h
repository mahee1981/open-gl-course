#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class BaseLight
{
public:
	BaseLight();
	BaseLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity);
	BaseLight(glm::vec3 color, GLfloat aIntensity, GLfloat diffuseIntensity);
	~BaseLight();


protected:
	glm::vec3 color;
	GLfloat ambientIntesity;
	GLfloat diffuseIntensity;
	void virtual UseLight(GLint ambientIntensityLocation, GLint ambientColorLocation, GLint diffuseIntensityLocation) const;
};

