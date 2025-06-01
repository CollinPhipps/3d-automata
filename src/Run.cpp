#include "Run.h"
#include <iostream>

#include <algorithm>

#include "GLFW/glfw3.h"

Run::Run()
	: m_ibo(m_cellIndices.data(), 36), m_vbo(m_cellVertices.data(), 56 * sizeof(float)),
	m_shader("../shaders/Cell.shader"), m_instanceVBO(m_instances.data(), m_cellCount * sizeof(glm::vec4)), m_model(1.0f),
	m_instances(m_cellCount), m_cubeShader("../shaders/Cube.shader"), m_cubeVBO(m_cubeVertices.data(), 72 * sizeof(float))
{
	VertexBufferLayout cellLayout;
	cellLayout.push<float>(3);
	m_vao.addBuffer(m_vbo, cellLayout, 0);

	VertexBufferLayout instanceLayout;
	instanceLayout.push<float>(4, 1);
	m_vao.addBuffer(m_instanceVBO, instanceLayout, 1);

	glm::mat4 mvp = m_proj * m_view * m_model;
	m_shader.bind();
	m_shader.setUniformMat4f("u_MVP", mvp);
	m_shader.setUniform1f("u_maxDecay", static_cast<float>(m_userRules.decayTime));

	VertexBufferLayout cubeLayout;
	cubeLayout.push<float>(3);
	m_cubeVAO.addBuffer(m_cubeVBO, cubeLayout, 0);
	m_cubeShader.bind();
	m_cubeShader.setUniformMat4f("u_MVP", mvp);

	initInstances();
	m_instanceVBO.bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_cellCount * sizeof(glm::vec4), m_instances.data()));
}

void Run::initInstances()
{
	int index = 0;
	float spacing = 0.1f;
	int halfGridSize = 20;

	for (int y = -halfGridSize; y < halfGridSize; ++y) {
		for (int x = -halfGridSize; x < halfGridSize; ++x) {
			for (int z = -halfGridSize; z < halfGridSize; ++z) {
				glm::vec4 instance;
				instance.x = x * spacing;
				instance.y = y * spacing;
				instance.z = z * spacing;

				float dist = glm::length(glm::vec3(instance.x, instance.y, instance.z));

				float state = 0.0f;

				float width = m_patternRules.radius * 4;

				switch (m_currentPattern) {
				case InitPattern::SphereRandom:
					if (dist <= m_patternRules.radius && (rand() % 100) < m_patternRules.fillPercent)
						state = 1.0f;
					break;

				case InitPattern::CubeRandom:
					if (abs(x) < width && abs(y) < width && abs(z) < width && (rand() % 100) < m_patternRules.fillPercent)
						state = 1.0f;
					break;

				case InitPattern::SymmetricNoise:
					if (abs(x + y + z) < m_patternRules.radius * 10 && (rand() % 100) < m_patternRules.fillPercent)
						state = 1.0f;
					break;
				}

				instance.w = state;
				m_instances[index++] = instance;
			}
		}
	}
}

void Run::initImGui(GLFWwindow*& window)
{
	ImGuiContext* context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init();
	m_io = &ImGui::GetIO();
}

void Run::shutdownImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Run::renderImGui(glm::quat& rotation, float& zoom, float deltaTime, GLFWwindow* window)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool showMainWindow = true;
	ImGui::Begin("Controls", &showMainWindow);

	imGuiFunctionality(zoom, deltaTime, window);
	updateRotation(deltaTime, window, rotation);

	ImGui::End();
	if (!showMainWindow)
		glfwSetWindowShouldClose(window, true);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Run::imGuiFunctionality(float& zoom, float deltaTime, GLFWwindow* window)
{
	if (ImGui::Button("Play", ImVec2(50, 25)))
	{
		m_isPlaying = true;
	}

	if (ImGui::Button("Pause", ImVec2(50, 25)))
	{
		m_isPlaying = false;
	}

	if (ImGui::Button("Reset", ImVec2(50, 25)))
	{
		m_isPlaying = false;
		initInstances();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_cellCount * sizeof(glm::vec4), m_instances.data()));
	}

	ImGui::SliderInt("Birth Neighbors", &m_userRules.birthNeighbors, 0, 20);
	ImGui::SliderInt("Survive Neighbors", &m_userRules.surviveNeighbors, 0, 20);
	ImGui::SliderInt("Decay Time", &m_userRules.decayTime, 0, 20);
	m_shader.bind();
	m_shader.setUniform1f("u_maxDecay", static_cast<float>(m_userRules.decayTime));

	const char* patternNames[] = { "Sphere Random", "Cube Random", "Symmetric Noise" };
	int currentPatternIndex = static_cast<int>(m_currentPattern);

	ImGui::Combo("Init Pattern", &currentPatternIndex, patternNames, IM_ARRAYSIZE(patternNames));
	m_currentPattern = static_cast<InitPattern>(currentPatternIndex);

	ImGui::SliderFloat("Init Radius", &m_patternRules.radius, 0.1f, 1.5f);
	ImGui::SliderInt("Fill %", &m_patternRules.fillPercent, 0, 100);

	float scrollY = m_io->MouseWheel;
	float zoomSpeed = 1.0f;

	zoom -= scrollY * zoomSpeed;
	zoom = std::clamp(zoom, 15.0f, 90.0f);

	m_proj = glm::perspective(glm::radians(zoom), 1280.f / 720.f, 0.1f, 100.f);
}

int Run::countLiveNeighbors(const std::vector<glm::vec4>& grid, int x, int y, int z)
{
	int liveCount = 0;
	for (int dz = -1; dz <= 1; dz++) 
	{
		for (int dy = -1; dy <= 1; dy++) 
		{
			for (int dx = -1; dx <= 1; dx++) 
			{
				if (dx == 0 && dy == 0 && dz == 0) continue;

				int nx = x + dx;
				int ny = y + dy;
				int nz = z + dz;

				if (nx >= 0 && nx < 40 && ny >= 0 && ny < 40 && nz >= 0 && nz < 40) 
				{
					int index = nx + ny * 40 + nz * 40 * 40;
					if (grid[index].w == 1.0f) liveCount++;
				}
			}
		}
	}
	return liveCount;
}

void Run::updateInstances()
{
	std::vector<glm::vec4> prevGrid = m_instances;

	for (int z = 0; z < 40; z++) 
	{
		for (int y = 0; y < 40; y++) 
		{
			for (int x = 0; x < 40; x++) 
			{
				int index = x + y * 40 + z * 40 * 40;
				int currentState = static_cast<int>(prevGrid[index].w);
				int liveNeighbors = countLiveNeighbors(prevGrid, x, y, z);

				if (currentState == 0) 
				{
					m_instances[index].w = (liveNeighbors == m_userRules.birthNeighbors) ? 1.0f : 0.0f;
				}
				else if (currentState == 1) 
				{
					m_instances[index].w = (liveNeighbors == m_userRules.surviveNeighbors) ? 1.0f : (m_userRules.decayTime > 0 ? 2.0f : 0.0f);
				}
				else if (currentState > 1) 
				{
					int decayLeft = static_cast<int>(prevGrid[index].w);
					m_instances[index].w = (decayLeft < m_userRules.decayTime + 1) ? decayLeft + 1.0f : 0.0f;
				}
			}
		}
	}
}

void Run::draw()
{
	m_renderer.drawInstanced(m_vao, m_ibo, m_shader, m_cellCount);
	m_renderer.drawLines(m_cubeVAO, m_cubeShader, 24);
}

void Run::updateMatrices()
{
	glm::mat4 mvp = m_proj * m_view * m_model;
	m_shader.bind();
	m_shader.setUniformMat4f("u_MVP", mvp);
	m_cubeShader.bind();
	m_cubeShader.setUniformMat4f("u_MVP", mvp);
}

float Run::updateInstanceBuffer(float currentTime, float lastUpdateTime)
{
	float updateInterval = 0.1f;
	if (currentTime - lastUpdateTime >= updateInterval)
	{
		updateInstances();
		m_instanceVBO.bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_cellCount * sizeof(glm::vec4), m_instances.data()));
		lastUpdateTime = currentTime;
	}
	
	return lastUpdateTime;
}

void Run::clear()
{
	m_renderer.clear();
}

bool Run::getState() const
{
	return m_isPlaying;
}

void Run::updateRotation(float deltaTime, GLFWwindow* window, glm::quat& rotation)
{
	constexpr float rotationSpeed = glm::radians(90.0f);

	glm::quat incrementalRotation = glm::quat(1, 0, 0, 0);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::quat delta = glm::angleAxis(-rotationSpeed * deltaTime, glm::vec3(0, 1, 0));
		incrementalRotation = delta * incrementalRotation;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::quat delta = glm::angleAxis(rotationSpeed * deltaTime, glm::vec3(0, 1, 0));
		incrementalRotation = delta * incrementalRotation;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::quat delta = glm::angleAxis(-rotationSpeed * deltaTime, glm::vec3(1, 0, 0));
		incrementalRotation = delta * incrementalRotation;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::quat delta = glm::angleAxis(rotationSpeed * deltaTime, glm::vec3(1, 0, 0));
		incrementalRotation = delta * incrementalRotation;
	}

	rotation = incrementalRotation * rotation;

	m_model = glm::mat4_cast(rotation);
}