#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


// Just an array of vertex positions 
GLfloat vertices[] = {
//   COORDINATES         COLORS             TEXTURE COORDINATES		
	-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2,
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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8*sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8*sizeof(float), (void*)(3*sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8*sizeof(float), (void*)(6*sizeof(float)));

	// Unbind all the objects
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Create a reference to the uniform variable "scale" in the shader program
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("grassBlock.jpg", &width, &height, &nrChannels, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);


	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
	shaderProgram.Activate();
	glUniform1i(tex0Uni, 0);


	// Terminate the function only on closing the window
	while (!glfwWindowShouldClose(window)) {
		//Clean the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Enable the shader program
		shaderProgram.Activate();

		// Set the uniform variable "scale" to 0.5
		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Bind the vertex array
		VAO1.Bind();

		// Draw the bound array(s)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap buffers to render
		glfwSwapBuffers(window);

		// Take care of GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	glDeleteTextures(1, &texture);
	shaderProgram.Delete();

	// Then destroy the window and terminate glfw.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}