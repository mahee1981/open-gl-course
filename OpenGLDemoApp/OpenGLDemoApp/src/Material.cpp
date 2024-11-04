#include "../include/Material.h"

Material::Material()
{
	this->specularIntensity = 0.0f;
	this->shininess = 0.0f;
}

Material::Material(GLfloat specularIntensity, GLfloat shininess)
{
	this->specularIntensity = specularIntensity;
	this->shininess = shininess;
}

Material::~Material()
{
}

void Material::UseMaterial(GLint specularIntensityLocation, GLint shininessLocation) const
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}
