#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include"model.h"
#include"FBO.h"
#include"screenQuad.h"
#include "Main.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

bool isImpacting = false;

float gamma = 2.2f;

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

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
	GLint effectType = 0;

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

	double iPressedTime = 0.0f;

	float randMin = 0;
	float randMax = 1;

	bool simulateIpress = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window)) {

		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;
		counter++;

		if (deltaTime >= 1.0 / 60.0) {
			std::string FPS = std::to_string((1.0 / deltaTime) * counter);
			std::string ms = std::to_string((deltaTime / counter) * 1000.0);
			std::string newTitle = "ImpactFrames | FPS: " + FPS + " | ms: " + ms;
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = currTime;
			counter = 0;

			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS || simulateIpress) {
				isImpacting = true;
				iPressedTime += deltaTime;
			} else {
				isImpacting = false;
				iPressedTime = 0.0f;
			}
		}

		postProcessingFBO.bind();

		//Clean the background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (!io.WantCaptureMouse) {
			camera.Inputs(window);
		}

		glEnable(GL_DEPTH_TEST);

		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		// define a vector that returns a value between two set variables

		glm::vec3 randomOffset = glm::vec3(
			RandomFloat(randMin, randMax),
			RandomFloat(randMin, randMax),
			RandomFloat(randMin, randMax)
		);

		shaderProgram.Activate();
		if (counter == 0 && isImpacting == true) {
			glUniform3fv(glGetUniformLocation(shaderProgram.ID, "randomOffset"), 1, glm::value_ptr(randomOffset));
		}
		else if (counter == 0 && isImpacting == false){
			glUniform3fv(glGetUniformLocation(shaderProgram.ID, "randomOffset"), 1, glm::value_ptr(glm::vec3(0)));
		}
		if (isImpacting == true) {
			glUniform1f(glGetUniformLocation(shaderProgram.ID, "iPressTime"), iPressedTime);
		}
		else {
			glUniform1f(glGetUniformLocation(shaderProgram.ID, "iPressTime"), 0.0f);
		}
		model.Draw(shaderProgram, camera);
		
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		outliningProgram.Activate();
		glUniform1f(glGetUniformLocation(outliningProgram.ID, "scale"), scale);
		if (isImpacting == true) {
			glUniform1f(glGetUniformLocation(outliningProgram.ID, "outline"), 0.1f);
			glUniform1f(glGetUniformLocation(outliningProgram.ID, "seed"), currTime);
			glUniform1f(glGetUniformLocation(outliningProgram.ID, "iPressTime"), iPressedTime);
			glUniform1i(glGetUniformLocation(outliningProgram.ID, "isImpacting"), 1);
		}
		else {
			glUniform1f(glGetUniformLocation(outliningProgram.ID, "outline"), 0.0f);
			glUniform1i(glGetUniformLocation(outliningProgram.ID, "isImpacting"), 0 );
			glUniform1f(glGetUniformLocation(outliningProgram.ID, "iPressTime"), 0);
		}
		model.Draw(outliningProgram, camera);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// second pass
		postProcessingFBO.unbind();

		float timeSin = glm::sin(( 0.2f + iPressedTime) * 2);
		//clamp timeSin between 0 and 1
		timeSin = glm::clamp(timeSin, 0.0f, 1.0f);

		framebufferProgram.Activate();
		glUniform1i(glGetUniformLocation(framebufferProgram.ID, "effectType"), effectType);
		glUniform1f(glGetUniformLocation(framebufferProgram.ID, "gamma"), gamma);
		if (isImpacting == true) {
			glUniform1i(glGetUniformLocation(framebufferProgram.ID, "isImpacting"), 1);
			glUniform1f(glGetUniformLocation(framebufferProgram.ID, "barrelPower"), timeSin);
			glUniform1f(glGetUniformLocation(framebufferProgram.ID, "iPressTime"), iPressedTime);
		}
		else {
			glUniform1i(glGetUniformLocation(framebufferProgram.ID, "isImpacting"), 0);
			glUniform1f(glGetUniformLocation(framebufferProgram.ID, "barrelPower"), 0);
			glUniform1f(glGetUniformLocation(framebufferProgram.ID, "iPressTime"), 0);
		}
		screenQuad.draw(postProcessingFBO.getColorAttachment());

		ImGui::Begin("Impact Settings");
		ImGui::SliderInt("Effect type", &effectType, 0, 5);
		ImGui::SliderFloat("Outline Scale", &scale, 0.0f, 0.07f);
		ImGui::SliderFloat("Screen Shake Amount", &randMax, 0.0f, 2.0f);
		ImGui::Checkbox("Simulate", &simulateIpress);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers to render
		glfwSwapBuffers(window);

		// Take care of GLFW events
		glfwPollEvents();
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	// Delete all the objects we've created
	shaderProgram.Delete();

	// Then destroy the window and terminate glfw.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}