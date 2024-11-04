#pragma once


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

/*
This camera system is a fly like camera that suits most purposes and works well with Euler angles,
but be careful when creating different camera systems like an FPS camera, or a flight simulation camera.
Each camera system has its own tricks and quirks so be sure to read up on them.
For example, this fly camera doesn't allow for pitch values higher than or equal to 90 degrees
and a static up vector of (0,1,0) doesn't work when we take roll values into account.
*/
class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat movementSpeed, GLfloat turnSpeed);
	void KeyControl(const bool* keys, GLfloat deltaTime);
	void MouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 GetCameraPosition() const;
	glm::vec3 GetCameraDirection() const;
	glm::mat4 CalculateViewMatrix() const;
	~Camera();

private:
	glm::vec3 position; //world position of the camera

	glm::vec3 front; // where is camera looking at
	glm::vec3 up; // up from camera
	glm::vec3 right; // right of camera

	glm::vec3 worldUp; // where the sky is in the world

	GLfloat yaw;
	GLfloat pitch;
	GLfloat movementSpeed;
	GLfloat turnSpeed;


	void update();

};

