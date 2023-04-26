#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


// Just an array of vertex positions 
GLfloat vertices[] = {
//        COORDINATES                               COLORS          		
	-0.5f,  -0.5f * float(sqrt(3)) / 3,      0.0f,  0.8f,  0.3f,  0.02f,
	 0.5f,  -0.5f * float(sqrt(3)) / 3,      0.0f,  0.8f,  0.3f,  0.02f,
	 0.0f,   0.5f * float(sqrt(3)) * 2 / 3,  0.0f,  1.0f,  0.6f,  0.32f,
	-0.25f,  0.5f * float(sqrt(3)) / 6,      0.0f,  0.9f,  0.45f, 0.17f,
	 0.25f,  0.5f * float(sqrt(3)) / 6,      0.0f,  0.9f,  0.45f, 0.17f,
	 0.0f,  -0.5f * float(sqrt(3)) / 3,      0.0f,  0.8f,  0.3f,  0.02f
};

GLuint indices[] =
{
	0, 3, 5,
	3, 2, 4,
	5, 4 ,1
};


int main() {
	// Initialize GLFW
	glfwInit();

	// Specify GLFW Version (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Only use core profile of OpenGL (modern functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window object 800x800
	GLFWwindow* window = glfwCreateWindow(800, 800, "ImpactFrames", NULL, NULL);
	
	// Check for failures
	if (window == NULL) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the context
	glfwMakeContextCurrent(window);

	// Load GLAD to configure OpenGL
	gladLoadGL();

	// Set the GL window size to our above window
	glViewport(0, 0, 800, 800);

	// Create the shader program using the default.vert and default.frag files
	Shader shaderProgram("default.vert", "default.frag");

	// Generate and Bind the Vertex Array Object
	VAO VAO1;
	VAO1.Bind();

	// Generate the Vertex Buffer Object and the Element Buffer Object
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Link the VBO to the VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6*sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6*sizeof(float), (void*)(3*sizeof(float)));

	// Unbind all the objects
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Create a reference to the uniform variable "scale" in the shader program
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Terminate the function only on closing the window
	while (!glfwWindowShouldClose(window)) {
		//Clean the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Enable the shader program
		shaderProgram.Activate();

		// Set the uniform variable "scale" to 0.5
		glUniform1f(uniID, 0.5f);

		// Bind the vertex array (unnecessary since we have only one object but hey)
		VAO1.Bind();

		// Draw the bound array(s)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		// Swap buffers to render
		glfwSwapBuffers(window);

		// Take care of GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Then destroy the window and terminate glfw.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}