#include "../include/DirectionalLight.h"


DirectionalLight::DirectionalLight() : BaseLight()
{
	this->direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity,
								   GLfloat xDirection, GLfloat yDirection, GLfloat zDirection) : BaseLight(red, green, blue, aIntensity, diffuseIntensity)
{
	this->direction = glm::vec3(xDirection, yDirection, zDirection);
}


DirectionalLight::DirectionalLight(glm::vec3 color, GLfloat aIntensity, GLfloat diffuseIntensity, glm::vec3 direction) : BaseLight(color, aIntensity, diffuseIntensity)
{
	this->direction = direction;
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::UseLight(GLint ambientIntensityLocation, GLint ambientColorLocation,
								GLint diffuseIntensityLocation, GLint directionLocation) const
{
	BaseLight::UseLight(ambientIntensityLocation, ambientColorLocation, diffuseIntensityLocation);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}
