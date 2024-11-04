#pragma once

#include "../include/PointLight.h"

class SpotLight
{
public:
	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, 
		GLfloat constant, GLfloat linear, GLfloat quadratic,
		GLfloat edge);
	~SpotLight();

	SpotLight(glm::vec3 color, GLfloat aIntensity, GLfloat diffuseIntensity, glm::vec3 position, glm::vec3 direction, glm::vec3 attenuationConstants, GLfloat edge);

	void UseLight(GLint ambientIntensityLocation, GLint ambientColorLocation,
		GLint diffuseIntensityLocation, GLint positionLocation, GLint directionLocation,
		GLint constantLocation, GLint linearLocation, GLint quadraticLocation,
		GLint edgeLocation) const;

	void SetPositionAndDirection(glm::vec3 pos, glm::vec3 dir);

private:
	glm::vec3 direction;
	GLfloat edge, processedEdge;
	PointLight pointLightComponent;
};

