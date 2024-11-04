#pragma once

#include "../include/BaseLight.h"

class DirectionalLight : BaseLight
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat diffuseIntensity);


	DirectionalLight(glm::vec3 color, GLfloat aIntensity,GLfloat diffuseIntensity, glm::vec3 direction);

	~DirectionalLight();

	void UseLight(GLint ambientIntensityLocation, GLint ambientColorLocation,
				  GLint diffuseIntensityLocation, GLint directionLocation) const;

private:
	glm::vec3 direction;

};

