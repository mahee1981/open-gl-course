#include "..\include\Shader.h"

Shader::Shader()
{
	shaderProgram = 0;
	u_Model = 0;
	u_Projection = 0;
	u_View = 0;
	pointLightCount = 0;
}

void Shader::CreateFromString(const char* vertexShaderCode, const char* fragmentShaderCode)
{
	CompileShader(vertexShaderCode, fragmentShaderCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexCode = ReadFile(vertexLocation);
	std::string fragmentCode = ReadFile(fragmentLocation);

	CreateFromString(vertexCode.c_str(), fragmentCode.c_str());
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileHandle(fileLocation, std::ios::in);

	if (!fileHandle.is_open()) {
		std::cout << "Failed to read " << fileLocation << ". File doesn't exist" << std::endl;
		return "";
	}

	std::string line;
	while (!fileHandle.eof()) {
		std::getline(fileHandle, line);
		content.append(line + "\n");
	}

	fileHandle.close();

	std::cout << "Successfully read Shader code from " << fileLocation << std::endl;
	return content;
}

GLuint Shader::GetProjectionLocation() const
{
	return GLuint(u_Projection);
}

GLuint Shader::GetModelLocation() const
{
	return GLuint(u_Model);
}

GLuint Shader::GetViewLocation() const
{
	return GLuint(u_View);
}

//GLuint Shader::GetAmbientIntensityLocation() const
//{
//	return GLuint(uniformDirectionalLight.AmbientIntensity);
//}
//
//GLuint Shader::GetAmbientColorLocation() const
//{
//	return GLuint(uniformDirectionalLight.Color);
//}
//
//GLuint Shader::GetDiffuseIntensityLocation() const
//{
//	return GLuint(uniformDirectionalLight.DiffuseIntensity);
//}
//
//GLuint Shader::GetDirectionLocation() const
//{
//	return GLuint(uniformDirectionalLight.Direction);
//}

GLuint Shader::GetSpecularIntensityLocation() const
{
	return GLuint(u_SpecularIntensity);
}
 
GLuint Shader::GetShininessLocation() const
{
	return GLuint(u_Shininess);
}

GLuint Shader::GetEyePositionLocation() const
{
	return GLuint(u_EyePosition);
}

void Shader::UseShader() const
{
	glUseProgram(shaderProgram);
}

void Shader::SetDirectionalLight(const DirectionalLight& dLight) const
{
	dLight.UseLight(u_DirectionalLight.AmbientIntensity, u_DirectionalLight.Color,
					u_DirectionalLight.DiffuseIntensity, u_DirectionalLight.Direction);
}

void Shader::SetPointLights(const std::vector<PointLight>& pointLights) const
{
	unsigned int actualNumberOfLights = std::min((const unsigned int)pointLights.size(), MAX_POINT_LIGHTS);

	glUniform1i(u_PointLightCount, actualNumberOfLights);

	for (std::size_t i = 0; i < actualNumberOfLights; i++) {
		pointLights[i].UseLight(u_PointLight[i].AmbientIntensity, u_PointLight[i].Color, u_PointLight[i].DiffuseIntensity,
								u_PointLight[i].Position,
								u_PointLight[i].Constant, u_PointLight[i].Linear, u_PointLight[i].Quadratic);

	}
}

void Shader::SetSpotLights(const std::vector<SpotLight>& spotLights) const
{
	unsigned int actualNumberOfLights = std::min((const unsigned int)spotLights.size(), MAX_SPOT_LIGHTS);
	
	glUniform1i(u_SpotLightCount, actualNumberOfLights);

	for (std::size_t i = 0; i < actualNumberOfLights; i++) {
		spotLights[i].UseLight(u_SpotLight[i].AmbientIntensity, u_SpotLight[i].Color, u_SpotLight[i].DiffuseIntensity,
			u_SpotLight[i].Position, u_SpotLight[i].Direction,
			u_SpotLight[i].Constant, u_SpotLight[i].Linear, u_SpotLight[i].Quadratic,
			u_SpotLight[i].Edge);

	}

}

void Shader::ClearShader()
{
	if (shaderProgram != 0)
	{
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}
}

Shader::~Shader()
{
	std::cout << "Shader destroyed" << std::endl;
	ClearShader();
}

void Shader::CompileShader(const char* vertexShaderCode, const char* fragmentShaderCode)
{
	shaderProgram = glCreateProgram();

	if (!shaderProgram) {
		printf("Error creating shader program\n");
		return;
	}

	printf("Compiling Vertex Shader...\n");
	AddShader(shaderProgram, vertexShaderCode, GL_VERTEX_SHADER);
	printf("Compiling Fragment Shader...\n");
	AddShader(shaderProgram, fragmentShaderCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		printf("Program linking failed: %s\n", errorLog);
	}


	glValidateProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		printf("Invalid program: %s\n", errorLog);
	}

	u_Model = glGetUniformLocation(shaderProgram, "u_model");
	u_Projection = glGetUniformLocation(shaderProgram, "u_projection");
	u_View = glGetUniformLocation(shaderProgram, "u_view");
	u_DirectionalLight.Color = glGetUniformLocation(shaderProgram, "u_directionalLight.base.color");
	u_DirectionalLight.AmbientIntensity = glGetUniformLocation(shaderProgram, "u_directionalLight.base.ambientIntensity");
	u_DirectionalLight.Direction = glGetUniformLocation(shaderProgram, "u_directionalLight.direction");
	u_DirectionalLight.DiffuseIntensity = glGetUniformLocation(shaderProgram, "u_directionalLight.base.diffuseIntensity");
	u_SpecularIntensity = glGetUniformLocation(shaderProgram, "u_material.specularIntensity");
	u_Shininess = glGetUniformLocation(shaderProgram, "u_material.shininess");
	u_EyePosition = glGetUniformLocation(shaderProgram, "u_eyePosition");
	u_PointLightCount = glGetUniformLocation(shaderProgram, "u_pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {
		std::stringstream myBuff;
		std::string valueSent;

		myBuff << "u_pointLights[" << i << "].base.color" << std::endl;
		std::getline(myBuff, valueSent, '\n');
		myBuff.clear();
		u_PointLight[i].Color = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_pointLights[" << i << "].base.ambientIntensity" << std::endl;;
		std::getline(myBuff, valueSent, '\n');
		myBuff.clear();

		u_PointLight[i].AmbientIntensity = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_pointLights[" << i << "].base.diffuseIntensity\n";
		std::getline(myBuff, valueSent);
		u_PointLight[i].DiffuseIntensity = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_pointLights[" << i << "].position\n";
		std::getline(myBuff, valueSent);
		u_PointLight[i].Position = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_pointLights[" << i << "].constant\n";
		std::getline(myBuff, valueSent);
		u_PointLight[i].Constant = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_pointLights[" << i << "].linear\n";
		std::getline(myBuff, valueSent);
		u_PointLight[i].Linear = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_pointLights[" << i << "].quadratic\n";
		std::getline(myBuff, valueSent);
		u_PointLight[i].Quadratic = glGetUniformLocation(shaderProgram, valueSent.c_str());

	}
	u_SpotLightCount = glGetUniformLocation(shaderProgram, "u_spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {
		std::stringstream myBuff;
		std::string valueSent;

		myBuff << "u_spotLights[" << i << "].base.base.color" << std::endl;
		std::getline(myBuff, valueSent, '\n');
		u_SpotLight[i].Color = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_spotLights[" << i << "].base.base.ambientIntensity" << std::endl;;
		std::getline(myBuff, valueSent, '\n');
		u_SpotLight[i].AmbientIntensity = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_spotLights[" << i << "].base.base.diffuseIntensity\n";
		std::getline(myBuff, valueSent);
		u_SpotLight[i].DiffuseIntensity = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_spotLights[" << i << "].base.position\n";
		std::getline(myBuff, valueSent);
		u_SpotLight[i].Position = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_spotLights[" << i << "].base.constant\n";
		std::getline(myBuff, valueSent);
		u_SpotLight[i].Constant = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_spotLights[" << i << "].base.linear\n";
		std::getline(myBuff, valueSent);
		u_SpotLight[i].Linear = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_spotLights[" << i << "].base.quadratic\n";
		std::getline(myBuff, valueSent);
		u_SpotLight[i].Quadratic = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_spotLights[" << i << "].direction\n";
		std::getline(myBuff, valueSent);
		u_SpotLight[i].Direction = glGetUniformLocation(shaderProgram, valueSent.c_str());

		myBuff << "u_spotLights[" << i << "].edge\n";
		std::getline(myBuff, valueSent);
		u_SpotLight[i].Edge = glGetUniformLocation(shaderProgram, valueSent.c_str());

	}
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);

}

void Shader::AddShader(GLuint shaderProgramId, const char* shaderProgram, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	glShaderSource(theShader, 1, &shaderProgram, NULL);
	glCompileShader(theShader);


	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(errorLog), NULL, errorLog);
		printf("Shader compilation failed: %s\n", errorLog);
	}

	glAttachShader(shaderProgramId, theShader);
}
