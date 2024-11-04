#include "../include/MyWindow.h"
#include <iostream>

MyWindow::MyWindow()
{
	width = 800;
	height = 600;

	bufferWidth = bufferHeight = 0;
	mainWindow = nullptr;
	mouseFirstMoved = true;


	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; ++i)
	{
		keys[i] = false;
	}
}

MyWindow::MyWindow(GLint windowWidth, GLint windowHeight)
{

	this->width = windowWidth;
	this->height = windowHeight;

	mouseFirstMoved = true;
	bufferWidth = bufferHeight = 0;
	mainWindow = nullptr;

	lastX = 0.0f;
	lastY = 0.0f;
	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < 1024; ++i)
	{
		keys[i] = false;
	}
}

int MyWindow::Initialize()
{
	if (!glfwInit())
	{
		printf("GLFW initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;

	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	printf("Width = %d, Height = %d\n", bufferWidth, bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Handle Key and Mouse input
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup ViewPort size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//enable depth test (zbuffer)
	glEnable(GL_DEPTH_TEST);

	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

GLfloat MyWindow::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat MyWindow::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

MyWindow::~MyWindow()
{
	std::cout << "Window destroyed" << std::endl;
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void MyWindow::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void MyWindow::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	MyWindow* theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {

		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;

		}
	}
}

void MyWindow::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	MyWindow* theWindow = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved) {
		theWindow->lastX = (GLfloat)xPos;
		theWindow->lastY = (GLfloat)yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = (GLfloat)xPos - theWindow->lastX;
	theWindow->yChange = (GLfloat)yPos - theWindow->lastY;

	theWindow->lastX = (GLfloat)xPos;
	theWindow->lastY = (GLfloat)yPos;

}
