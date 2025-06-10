#include "glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "Run.h"

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "Could not glfw init\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	window = glfwCreateWindow(1280, 720, "3D Automata", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "Could not create window\n";
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height) 
	{
		glViewport(0, 0, width, height);
	});

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		return -1;
	}

	Run simulation = Run();
	simulation.initImGui(window);

	float radiansY = 0.0f;
	float radiansX = 0.0f;

	glm::quat rotation = glm::quat(1, 0, 0, 0);

	float lastFrameTime = 0.0f;
	float lastUpdateTime = 0.0f;

	float zoom = 0.0f;

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		simulation.clear();

		float currentFrameTime = glfwGetTime();
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		simulation.renderImGui(rotation, zoom, deltaTime, window);
		simulation.updateMatrices();

		simulation.draw();

		if (simulation.getState())
		{
			lastUpdateTime = simulation.updateInstanceBuffer(currentFrameTime, lastUpdateTime);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	simulation.shutdownImGui();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}