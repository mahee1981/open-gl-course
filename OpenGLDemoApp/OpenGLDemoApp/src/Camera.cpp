#include "../include/Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat movementSpeed, GLfloat turnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;

	update();
}

void Camera::KeyControl(const bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W] == true) {
		position += front * velocity;

	}
	if (keys[GLFW_KEY_S] == true) {
		position -= front * velocity;

	}
	if (keys[GLFW_KEY_A] == true) {
		position += right * velocity;

	}
	if (keys[GLFW_KEY_D] == true) {
		position -= right * velocity;
	}
}

void Camera::MouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	update();
}

glm::vec3 Camera::GetCameraPosition() const
{
	return position;
}

glm::vec3 Camera::GetCameraDirection() const
{
	return glm::normalize(front);
}

glm::mat4 Camera::CalculateViewMatrix() const
{
	return glm::lookAt(position, position + front, worldUp);
}

Camera::~Camera()
{
	std::cout << "Camera destroyed" << std::endl;
}

void Camera::update()
{
	front.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	front.y = sinf(glm::radians(pitch));
	front.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(worldUp, front));
	up = glm::normalize(glm::cross(front, right));

}

