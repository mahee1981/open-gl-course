#include "../include/PointLight.h"

PointLight::PointLight() : BaseLight()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->constant = 1.0f;
	this->linear = 0.0f;
	this->quadratic = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat constant, GLfloat linear, GLfloat quadratic)
	: BaseLight(red, green, blue, aIntensity, diffuseIntensity)
{
	this->position = glm::vec3(xPos, yPos, zPos);
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

PointLight::PointLight(glm::vec3 color, GLfloat aIntensity, GLfloat diffuseIntensity, glm::vec3 position, glm::vec3 attenuationConstants) : BaseLight(color, aIntensity, diffuseIntensity)
{
	this->position = position;
	constant = attenuationConstants.x;
	linear = attenuationConstants.y;
	quadratic = attenuationConstants.z;
}

void PointLight::UseLight(GLint ambientIntensityLocation, GLint ambientColorLocation, GLint diffuseIntensityLocation,
	GLint positionLocation, GLint constantLocation, GLint linearLocation, GLint quadraticLocation) const
{
	BaseLight::UseLight(ambientIntensityLocation, ambientColorLocation, diffuseIntensityLocation);
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(quadraticLocation, quadratic);
}

void PointLight::SetLightPosition(glm::vec3 position)
{
	this->position = position;
}

PointLight::~PointLight()
{
}
