#include"model.h"
#include"FBO.h"
#include"screenQuad.h"
#include "Main.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main() {
	// Initialize GLFW
	glfwInit();

	// Specify GLFW Version (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Only use core profile of OpenGL (modern functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window object 800x800
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ImpactFrames", NULL, NULL);
	
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
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Create the shader program using the default.vert and default.frag files
	Shader shaderProgram("default.vert", "default.frag");
	Shader outliningProgram("outline.vert", "outline.frag");
	Shader framebufferProgram("framebuffer.vert", "framebuffer.frag");
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	GLfloat scale = 0.05f;

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "scale"), scale);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.3f, 1.3f));

	Model model("models/destiny/hunter.gltf");

	Framebuffer postProcessingFBO = Framebuffer(SCR_WIDTH, SCR_HEIGHT);
	ScreenQuad screenQuad = ScreenQuad();

	double prevTime = 0.0f;
	double currTime = 0.0f;
	double deltaTime = 0.0f;
	unsigned int counter = 0;

	while (!glfwWindowShouldClose(window)) {

		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;
		counter++;

		if (deltaTime >= 1.0 / 30.0) {
			std::string FPS = std::to_string((1.0 / deltaTime) * counter);
			std::string ms = std::to_string((deltaTime / counter) * 1000.0);
			std::string newTitle = "ImpactFrames | FPS: " + FPS + " | ms: " + ms;
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = currTime;
			counter = 0;
		}

		postProcessingFBO.bind();

		//Clean the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		model.Draw(shaderProgram, camera);
		
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		outliningProgram.Activate();
		glUniform1f(glGetUniformLocation(outliningProgram.ID, "scale"), scale);
		glUniform1f(glGetUniformLocation(outliningProgram.ID, "outline"), 0.1f);
		model.Draw(outliningProgram, camera);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// second pass
		postProcessingFBO.unbind();

		framebufferProgram.Activate();
		screenQuad.draw(postProcessingFBO.getColorAttachment());

		// Swap buffers to render
		glfwSwapBuffers(window);

		// Take care of GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();

	// Then destroy the window and terminate glfw.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}