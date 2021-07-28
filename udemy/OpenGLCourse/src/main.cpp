#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;
const float toRadians = 3.14159265f /180.0f;

GLuint VAO, VBO, shader, uniformModel;

bool horizontalDirection = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleIncrement = 0.02f;
float currentAngle = 0.0f;

bool sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vertexShader = "          \n\
#version 330                                 \n\
                                             \n\
layout (location = 0) in vec3 pos;           \n\
                                             \n\
uniform mat4 model;                          \n\
                                             \n\
void main()                                  \n\
{                                            \n\
                                             \n\
	gl_Position = model * vec4(pos, 1.0);    \n\
}";

static const char* fragmentShader = "        \n\
#version 330                                 \n\
                                             \n\
out vec4 color;                              \n\
                                             \n\
void main()                                  \n\
{                                            \n\
	color = vec4(0.3, 0.0, 0.9, 1.0);        \n\
}";

void CreateTriangle() 
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void AddShader(GLuint program, const char* sourceCode, GLenum shaderType) 
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = sourceCode; 

	GLint codeLength[1];
	codeLength[0] = strlen(sourceCode);

	glShaderSource(theShader, 1, code, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s' \n", shaderType, eLog);
		return;
	}

	glAttachShader(program, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Error creating the shader program!\n");
		return;
	}

	AddShader(shader, vertexShader, GL_VERTEX_SHADER);
	AddShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if(!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking the program: '%s' \n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating the program: '%s' \n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
}

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

	CreateTriangle();
	CompileShaders();

	// Run Loop
	while(!glfwWindowShouldClose(mainWindow))
	{
		// Get and Handle user input events
		glfwPollEvents();
		
		if(horizontalDirection)
			triangleOffset += triangleIncrement;
		else 
			triangleOffset -= triangleIncrement;

		if(abs(triangleOffset) >= triangleMaxOffset)
			horizontalDirection = !horizontalDirection;
		
		currentAngle += 0.5f;
		if(currentAngle >= 360)
			currentAngle -= 360;
		
		if (horizontalDirection)
			currentSize += 0.01f;
		else
			currentSize -= 0.01f;

		if (currentSize >= maxSize || currentSize <= minSize)
			sizeDirection = !sizeDirection;

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		
		glm::mat4 model(1.0f);
		
		model = glm::translate(model, glm::vec3(triangleOffset, 0.0f, 0.0f));
		//model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(currentSize, 0.4f , 1.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}

