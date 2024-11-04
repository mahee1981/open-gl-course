#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class MyWindow
{
public:
	MyWindow();

	MyWindow(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLfloat GetBufferWidth() const { return (GLfloat)bufferWidth; }
	GLfloat GetBufferHeight() const { return (GLfloat)bufferHeight; }

	bool getShouldClose() const { return glfwWindowShouldClose(mainWindow); }
	const bool* getKeys() const { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~MyWindow();

private:

	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1204];

	void createCallbacks();

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	bool mouseFirstMoved;

	static void HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};

