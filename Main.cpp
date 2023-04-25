#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() {
	// Initialize GLFW
	glfwInit();

	// Specify GLFW Version (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Only use core profile of OpenGL (modern functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Just an array of vertex positions 
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};

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

	// Define a reference to a vertexShader (all OpenGL objects are accessed by a handle (aka reference))
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Set the shader source of the above defined reference to our string at file start. The one is the amount of strings.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the shader into machine code before running.
	glCompileShader(vertexShader);

	// Same
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Define a reference to a Shader Program
	GLuint shaderProgram = glCreateProgram();

	// Attach both shaders to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Link the program
	glLinkProgram(shaderProgram);

	// Once inserted into the program, shader references can be dumped
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// A buffer object is a chunk of data that is transmitted from the CPU to the GPU
	// Define both Vertex Array object and Vertex Buffer object's references
	GLuint VAO, VBO;

	// VAO is always generated before the VBO as it has to hold a reference and info on how to interpret the VBO
	// Generate the arrays and the buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind both the vertex array and vertex buffer so that next modifications will affect them
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Set the buffer data to the above specified vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set the position of the vertices as the first attribute of such vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Set the clear background color of the viewport
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// Clean the back buffer 
	glClear(GL_COLOR_BUFFER_BIT);

	// Swap the buffers to render the color
	glfwSwapBuffers(window);

	// Terminate the function only on closing the window
	while (!glfwWindowShouldClose(window)) {
		//Clean the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Enable the shader program
		glUseProgram(shaderProgram);

		// Bind the vertex array (unnecessary since we have only one object but hey)
		glBindVertexArray(VAO);

		// Draw the bound array(s)
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers to render
		glfwSwapBuffers(window);

		// Take care of GLFW events
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Then destroy the window and terminate glfw.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}