#include "../include/BaseLight.h"

BaseLight::BaseLight()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	this->ambientIntesity = 1.0f;
	this->diffuseIntensity = 0.0f;
}

BaseLight::BaseLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity)
{

	color = glm::vec3(red, green, blue);
	this->ambientIntesity = aIntensity;
	this->diffuseIntensity = diffuseIntensity;

}

BaseLight::BaseLight(glm::vec3 color, GLfloat aIntensity, GLfloat diffuseIntensity)
{
	this->color = color;
	this->ambientIntesity = aIntensity;
	this->diffuseIntensity = diffuseIntensity;
}

BaseLight::~BaseLight()
{
}

void BaseLight::UseLight(GLint ambientIntensityLocation, GLint ambientColorLocation, GLint diffuseIntensityLocation) const
{
	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntesity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

