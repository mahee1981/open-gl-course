#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>


#include "../include/CommonValues.h"
#include "../include/PointLight.h"
#include "../include/SpotLight.h"
#include "../include/Mesh.h"
#include "../include/Shader.h"
#include "../include/MyWindow.h"
#include "../include/Camera.h"
#include "../include/Texture.h"
#include "../include/DirectionalLight.h"
#include "../include/Material.h"
#include "../include/Model.h"


//everything is pointers because memory on the GPU is not managed.
std::vector<std::unique_ptr<Mesh>> meshList;
std::vector<std::unique_ptr<Shader>> shaderList;


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
 
static const char* vertexShaderCode = "Shaders/shader.vert";

static const char* fragmentShaderCode = "Shaders/shader.frag";

static const char* brickTextureLocation = "Textures/red_bricks.png";
static const char* plainTextureLocation = "Textures/plain.png";
static const char* kebabTextureLocation = "Textures/kebab.png";

void calcAverageNormals(const std::vector<unsigned int>& indices, std::vector<float>& vertices, unsigned int vertexLength, unsigned int normalOffset)
{

	for (std::size_t i = 0; i < indices.size(); i += 3)
	{
		unsigned int in0 = indices[i] * vertexLength;
		unsigned int in1 = indices[i + 1] * vertexLength;
		unsigned int in2 = indices[i + 2] * vertexLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 vertexNormal = glm::normalize(glm::cross(v1, v2));

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += vertexNormal.x; vertices[in0 + 1] += vertexNormal.y; vertices[in0 + 2] += vertexNormal.z;
		vertices[in1] += vertexNormal.x; vertices[in1 + 1] += vertexNormal.y; vertices[in1 + 2] += vertexNormal.z;
		vertices[in2] += vertexNormal.x; vertices[in2 + 1] += vertexNormal.y; vertices[in2 + 2] += vertexNormal.z;
	}

	for (std::size_t i = 0; i < vertices.size() / vertexLength; i++) 
	{
		unsigned int nOffset = i * vertexLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateTriangle()
{
	std::vector<unsigned int> indices = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	std::vector<float> vertices = {
	//	x		y		z	  u,	v	  nx	ny    nz
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,   0.5f, 1.0f,  0.0f, 0.0f, 0.0f,
	};

	std::vector<unsigned int> floorIndices = {
		0, 2, 1,
		1, 2, 3
	};

	std::vector<float> floorVertices = {
		//	x		y		z	  u,	v			nx	ny    nz
			-10.0f, 0.0f, -10.0f, 0.0f, 0.0f,	 0.0f, -1.0f, 0.0f,
			10.0f, 0.0f, -10.0f,  10.0f, 0.0f,	 0.0f, -1.0f, 0.0f,
			-10.0f, 0.0f, 10.0f,  0.0f, 10.0f,	 0.0f, -1.0f, 0.0f,
			10.0f, 0.0f, 10.0f,   10.0f, 10.0f,  0.0f, -1.0f, 0.0f,
	};

	calcAverageNormals(indices, vertices, 8, 5);

	//std::vector<float> cubeVertices = {
	//	//x      y       z	  u		v,
	//	-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, //bottom left
	//	 1.0f, -1.0f, -1.0f, 0.25f, 0.0f, //bottom right
	//	-1.0f,  1.0f, -1.0f, 0.0f, 0.5f, //top left
	//	 1.0f,  1.0f, -1.0f, 0.25f, 0.5f, // top right
	//	 //backside			 u		v,
	//	-1.0f, -1.0f, 1.0f,	0.5f, 0.0f,
	//	 1.0f, -1.0f, 1.0f,	0.75f, 0.0f,
	//	-1.0f,  1.0f, 1.0f,	0.5f, 0.5f,
	//	 1.0f,  1.0f, 1.0f,	0.75f, 0.5f
	//};

	//std::vector<unsigned int> cubeIndices =
	//{
	//	0, 1, 2,
	//	1, 3, 2,
	//	4, 5, 6,
	//	6, 5, 7,
	//	1, 3, 5,
	//	5, 3, 7,
	//	2, 6, 4,
	//	2, 0, 4,
	//	2, 6, 7,
	//	3, 7, 2,
	//	0, 1, 4,
	//	4, 5, 1
	//};


	std::unique_ptr<Mesh> obj1 = std::make_unique<Mesh>();
	obj1->CreateMesh(vertices, indices);
	meshList.push_back(std::move(obj1));

	std::unique_ptr<Mesh> obj2 = std::make_unique<Mesh>();
	obj2->CreateMesh(vertices, indices);
	meshList.push_back(std::move(obj2));


	std::unique_ptr<Mesh> floorMesh = std::make_unique<Mesh>();
	floorMesh->CreateMesh(floorVertices, floorIndices);
	meshList.push_back(std::move(floorMesh));


}

void CreateShaders()
{
	std::unique_ptr<Shader> shader1 = std::make_unique<Shader>();
	shader1->CreateFromFiles(vertexShaderCode, fragmentShaderCode);
	shaderList.push_back(std::move(shader1));
}


int main() {

	MyWindow mainWindow = MyWindow(1366, 768);
	mainWindow.Initialize();

	CreateTriangle();
	CreateShaders();

	Model cato;
	cato.LoadModel("Models/12221_Cat_v1_l3.obj");

	//we set the yaw to -90 degrees since a yaw of 0 degrees points in the positive direction of the x-axis
	//and our scene is set up so that it points in the negative direction of the z-axis
	Camera camera(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f }, -60.0f, 0.0f, 5.0f, 0.5f);

	DirectionalLight MainLight = DirectionalLight(glm::vec3{ 1.0f, 1.0f, 1.0f }, // color
												  0.4f, 0.3f, //ambient and diffuse
												  glm::vec3{ 0.0f, 0.0f , -1.0f }); // direction
	std::vector<PointLight> pointLights;

	//pointLights.push_back(PointLight(0.0f, 1.0f, 0.0f,
	//								0.1f, 0.1f,
	//								4.0f, 1.0f, 0.0f,
	//								1.0f, 0.2f, 0.1f));


	//pointLights.push_back(PointLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	0.0f, 0.0f, 0.0f,
	//	1.0f, 0.2f, 0.1f));

	//pointLights.push_back(PointLight(0.0f, 0.0f, 1.0f,
	//								0.1f, 0.1f,
	//								-4.0f, 2.0f, 0.0f,
	//								1.0f, 0.1f, 0.1f));

	std::vector<SpotLight> spotLights;

	spotLights.push_back(SpotLight(glm::vec3{ 1.0f, 1.0f, 1.0f }, //color
									0.0f, 2.0f, // ambient and diffuse
									glm::vec3{ 0.0f, 1.0f, 0.0f }, // position
									glm::vec3{ 0.0f, -1.0f, 0.0f }, // direction
									glm::vec3{ 1.0f, 0.2f, 0.1f }, //attenuationConstants
									20.0f)); //edge angle

	//spotLights.push_back(SpotLight(glm::vec3{ 1.0f, 1.0f, 1.0f }, //color
	//								0.0f, 1.0f, // ambient and diffuse
	//								glm::vec3{ 0.0f, -1.5f, 0.0f }, // position
	//								glm::vec3{ -100.0f, -1.0f, 0.0f }, // direction
	//								glm::vec3{ 1.0f, 0.0f, 0.0f }, //attenuationConstants
	//								20.0f)); //edge angle

	Material shinyMaterial(4.0f, 256);
	Material dullMaterial(0.3f, 4);

	Texture brickTexture = Texture(brickTextureLocation);
	brickTexture.LoadTextureWithAlpha();
	Texture kebabTexture = Texture(kebabTextureLocation);
	kebabTexture.LoadTextureWithAlpha();
	Texture plainTexture = Texture(plainTextureLocation);
	plainTexture.LoadTextureWithAlpha();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	GLuint uniformEyePosition = 0, uniformShininess = 0, uniformSpecularIntensity = 0;
	glm::mat4 projection(1.0f); // we need to create this once
	projection = glm::perspective(glm::radians(45.0f), mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	float currAngle = 0.0f;

	//rendering loop
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Get + Handle user input events
		glfwPollEvents();

		camera.KeyControl(mainWindow.getKeys(), deltaTime);
		camera.MouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currAngle += 0.1f;

		if (currAngle > 360.0f) {
			currAngle -= 360.0f;
		}

		shaderList[0]->UseShader();

		uniformModel = shaderList[0]->GetModelLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		//specular light
		uniformEyePosition = shaderList[0]->GetEyePositionLocation();
		uniformShininess = shaderList[0]->GetShininessLocation();
		uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();

		
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

		spotLights[0].SetPositionAndDirection(camera.GetCameraPosition(), camera.GetCameraDirection()); // enable for horror style flashlight movement

		shaderList[0]->SetDirectionalLight(MainLight);
		shaderList[0]->SetPointLights(pointLights);
		shaderList[0]->SetSpotLights(spotLights);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTextureOnTextureUnit(GL_TEXTURE0);
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		kebabTexture.UseTextureOnTextureUnit(GL_TEXTURE0);
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTextureOnTextureUnit(GL_TEXTURE0);
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0 + sinf(glfwGetTime()), 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(currAngle), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cato.RenderModel();



		glUseProgram(0);

		mainWindow.swapBuffers();
	}
	return 0;
}