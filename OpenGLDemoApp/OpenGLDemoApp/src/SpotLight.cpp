#include "../include/SpotLight.h"

SpotLight::SpotLight() : pointLightComponent()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	processedEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
					GLfloat aIntensity, GLfloat diffuseIntensity,
					GLfloat xPos, GLfloat yPos, GLfloat zPos,
					GLfloat xDir, GLfloat yDir, GLfloat zDir,
					GLfloat constant, GLfloat linear, GLfloat quadratic,
					GLfloat edge) : pointLightComponent(red, green, blue, aIntensity, diffuseIntensity, xPos, yPos, zPos, constant, linear, quadratic)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	this->edge = edge;
	this->processedEdge = cosf(glm::radians(edge));
}


SpotLight::~SpotLight()
{
}

SpotLight::SpotLight(glm::vec3 color, GLfloat aIntensity, GLfloat diffuseIntensity, glm::vec3 position, glm::vec3 direction, glm::vec3 attenuationConstants, GLfloat edge)
	: pointLightComponent(color, aIntensity, diffuseIntensity, position, attenuationConstants)
{
	this->direction = glm::normalize(direction);
	this->edge = edge;
	this->processedEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLint ambientIntensityLocation, GLint ambientColorLocation, GLint diffuseIntensityLocation,
						 GLint positionLocation, GLint directionLocation,
						 GLint constantLocation, GLint linearLocation, GLint quadraticLocation,
						 GLint edgeLocation) const
{
	pointLightComponent.UseLight(ambientIntensityLocation, ambientColorLocation, diffuseIntensityLocation, positionLocation, constantLocation, linearLocation, quadraticLocation);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::SetPositionAndDirection(glm::vec3 pos, glm::vec3 dir)
{
	this->pointLightComponent.SetLightPosition(pos);
	this->direction = dir;
}


