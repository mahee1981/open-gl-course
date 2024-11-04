#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include<GL/glew.h>
#include "../include/CommonValues.h"
#include "../include/DirectionalLight.h"
#include "../include/PointLight.h"
#include "../include/SpotLight.h"
#include <sstream>
#include <vector>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexShaderCode, const char* fragmentShaderCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	
	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation() const;
	GLuint GetModelLocation() const;
	GLuint GetViewLocation() const;
	//GLuint GetAmbientIntensityLocation() const;
	//GLuint GetAmbientColorLocation() const;
	//GLuint GetDiffuseIntensityLocation() const;
	//GLuint GetDirectionLocation() const;
	GLuint GetSpecularIntensityLocation() const;
	GLuint GetShininessLocation() const;
	GLuint GetEyePositionLocation() const;
	
	void UseShader() const;
	void SetDirectionalLight(const DirectionalLight &dLight) const;
	void SetPointLights(const std::vector<PointLight>& pointLights) const;
	void SetSpotLights(const std::vector<SpotLight>& pointLights) const;

	void ClearShader();

	~Shader();

private:
	int pointLightCount;

	GLuint shaderProgram, u_Projection, u_Model, u_View;
	GLuint u_SpecularIntensity, u_Shininess;
	GLuint u_EyePosition;

	struct DirectionalLightUniform
	{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;

		GLuint Direction;
	} u_DirectionalLight;

	GLuint u_PointLightCount;

	struct PointLightUniform
	{
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;

		GLuint Position;
		GLuint Constant;
		GLuint Linear;
		GLuint Quadratic;
	} u_PointLight[MAX_POINT_LIGHTS];

	GLuint u_SpotLightCount;

	struct SpotLightUniform : public PointLightUniform
	{
		GLuint Direction;
		GLuint Edge;
	} u_SpotLight[MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexShaderCode, const char* fragmentShaderCode);
	void AddShader(GLuint shaderProgramId, const char* shaderProgram, GLenum shaderType);


};

