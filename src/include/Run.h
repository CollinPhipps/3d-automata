#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <array>
#include <vector>

struct RuleSettings
{
	int birthNeighbors = 3;
	int surviveNeighbors = 2;
	int decayTime = 3;
};

enum class InitPattern
{
	SphereRandom,
	CubeRandom,
	SymmetricNoise
};

struct InitPatternRules
{
	float radius = 0.7f;
	int fillPercent = 30;
};

class Run
{
private:
	std::array<float, 56> m_cellVertices = {
		-0.05f, -0.05f,  0.05f,
		 0.05f, -0.05f,  0.05f,
		 0.05f,  0.05f,  0.05f,
		-0.05f,  0.05f,  0.05f,

		-0.05f, -0.05f, -0.05f,
		 0.05f, -0.05f, -0.05f,
		 0.05f,  0.05f, -0.05f,
		-0.05f,  0.05f, -0.05f
	};

	std::array<unsigned int, 36> m_cellIndices = {
		0, 1, 2,
		2, 3, 0,
  
		1, 6, 5,
		6, 1, 2,

		5, 4, 7,
		7, 6, 5,

		4, 3, 0,
		3, 4, 7,

		2, 7, 6,
		7, 2, 3,

		1, 4, 0,
		4, 1, 5
	};

	std::array<float, 72> m_cubeVertices = {
		-2.05f, -2.05f, -2.05f,   2.05f, -2.05f, -2.05f,
		 2.05f, -2.05f, -2.05f,   2.05f,  2.05f, -2.05f,
		 2.05f,  2.05f, -2.05f,  -2.05f,  2.05f, -2.05f,
		-2.05f,  2.05f, -2.05f,  -2.05f, -2.05f, -2.05f,

		-2.05f, -2.05f,  2.05f,   2.05f, -2.05f,  2.05f,
		 2.05f, -2.05f,  2.05f,   2.05f,  2.05f,  2.05f,
		 2.05f,  2.05f,  2.05f,  -2.05f,  2.05f,  2.05f,
		-2.05f,  2.05f,  2.05f,  -2.05f, -2.05f,  2.05f,

		-2.05f, -2.05f, -2.05f,  -2.05f, -2.05f,  2.05f,
		 2.05f, -2.05f, -2.05f,   2.05f, -2.05f,  2.05f,
		 2.05f,  2.05f, -2.05f,   2.05f,  2.05f,  2.05f,
		-2.05f,  2.05f, -2.05f,  -2.05f,  2.05f,  2.05f
	};

	VertexArray m_cubeVAO;
	VertexBuffer m_cubeVBO;
	Shader m_cubeShader;

	Renderer m_renderer;
	VertexArray m_vao;
	IndexBuffer m_ibo;
	VertexBuffer m_vbo;
	Shader m_shader;
	std::vector<glm::vec4> m_instances;
	VertexBuffer m_instanceVBO;
	const static unsigned int m_cellCount = 64000;

	glm::mat4 m_proj = glm::perspective(glm::radians(45.f), 1280.f / 720.f, 0.1f, 100.f);
	glm::mat4 m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -20.0f));
	glm::mat4 m_model;

	ImGuiIO* m_io = nullptr;

	bool m_isPlaying = false;

	RuleSettings m_userRules;

	InitPattern m_currentPattern = InitPattern::SphereRandom;
	InitPatternRules m_patternRules;

public:
	Run();
	void initInstances();
	void initImGui(GLFWwindow*& window);
	void shutdownImGui();
	void renderImGui(glm::quat& rotation, float& zoom, float deltaTime, GLFWwindow* window);
	void imGuiFunctionality(float& zoom, float deltaTime, GLFWwindow* window);
	int countLiveNeighbors(const std::vector<glm::vec4>& grid, int x, int y, int z);
	void updateInstances();
	void draw();
	void updateMatrices();
	float updateInstanceBuffer(float currentTime, float lastUpdateTime);
	void clear();
	bool getState() const;
	void updateRotation(float deltaTime, GLFWwindow* window, glm::quat& rotation);
};