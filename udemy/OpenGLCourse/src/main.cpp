#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

int main()
{
	// Initialise GLFW
	if(!glfwInit())
	{
		printf("GLFW initialisation failed");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW Window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Enabling foward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* mainWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Udemy Course", NULL, NULL);
	if(!mainWindow)
	{
		printf("GLFW Window creation failed");
		glfwTerminate();
		return 1;
	}
	
	// Get The Buffer size info
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the conetxt for GLEW
	glfwMakeContextCurrent(mainWindow);
	
	// Allow modern extension features
	glewExperimental = GL_TRUE;
	
	if(glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport Size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Run Loop
	while(!glfwWindowShouldClose(mainWindow))
	{
		// Get and Handle user input events
		glfwPollEvents();

		// Clear Window
		glClearColor(1.0f, 0.0f , 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}

