#include "Application.h"

#include <iostream>

#include <misc/cpp/imgui_stdlib.cpp>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

#include <GLFW/glfw3.h>

static GLuint g_QuadVAO = 0, g_QuadVBO = 0;
static GLuint g_QuadShader = 0;

const char* quadVert = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
out vec2 vUV;
void main() {
    vUV = aUV;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* quadFrag = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTex;
void main() {
    FragColor = texture(uTex, vUV);
}
)";

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) app->OnKey(key, scancode, action, mods);
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) app->OnMouseButton(button, action, mods);
}

static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) app->OnCursorPos(xpos, ypos);
}

static void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) app->OnWindowResize(width, height);
}

static void WindowCloseCallback(GLFWwindow* window)
{
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) app->OnWindowClose();
}
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app) app->OnFramebufferResize(width, height);
}

GLuint CompileShader(GLenum type, const char* src) {
	GLuint s = glCreateShader(type);
	glShaderSource(s, 1, &src, nullptr);
	glCompileShader(s);
	return s;
}

void InitFullscreenQuad() {
	if (g_QuadVAO) return;

	float quadVertices[] = {
		// positions   // uvs
		-1.f, -1.f,   0.f, 0.f,
		 1.f, -1.f,   1.f, 0.f,
		 1.f,  1.f,   1.f, 1.f,
		-1.f,  1.f,   0.f, 1.f
	};
	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

	glGenVertexArrays(1, &g_QuadVAO);
	glBindVertexArray(g_QuadVAO);

	glGenBuffers(1, &g_QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Shader
	GLuint vs = CompileShader(GL_VERTEX_SHADER, quadVert);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, quadFrag);
	g_QuadShader = glCreateProgram();
	glAttachShader(g_QuadShader, vs);
	glAttachShader(g_QuadShader, fs);
	glLinkProgram(g_QuadShader);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glBindVertexArray(0);
}

void DrawFullscreenQuad(unsigned int texture, int width, int height) {
	InitFullscreenQuad();

	glUseProgram(g_QuadShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(g_QuadShader, "uTex"), 0);

	glBindVertexArray(g_QuadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}


Application::Application(int width, int height, const std::string& title) {

	// Add ImGUI context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	SetDarkThemeColors();

	m_Window = std::make_unique<Window>(width, height, title);
	RegisterCallbacks();
	glfwSetWindowUserPointer(m_Window->GetNativeWindow(), this);


	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

}

void Application::RegisterCallbacks() {
	m_Window->SetKeyCallback(KeyCallback);
	m_Window->SetMouseButtonCallback(MouseButtonCallback);
	m_Window->SetCursorPosCallback(CursorPosCallback);
	m_Window->SetWindowSizeCallback(WindowSizeCallback);
	m_Window->SetFramebufferSizeCallback(FramebufferSizeCallback);
	m_Window->SetWindowCloseCallback(WindowCloseCallback);
}

void Application::SetDarkThemeColors() {

	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

}

Application::~Application() {}

void Application::Run() {
	while (!m_Window->ShouldClose())
	{
		OnUpdate();
		m_Window->SwapBuffers();
		m_Window->PollEvents();
	}
}

void Application::OnUpdate() {

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!m_ImageGen) {
		m_ImageGen = std::make_unique<Image>(256, 256);

		if (!m_ImageTexture) {
			glGenTextures(1, &m_ImageTexture);
			glBindTexture(GL_TEXTURE_2D, m_ImageTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_ImageGen->GetWidth(), m_ImageGen->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		}

	}

	if (m_ImageGen && m_ImageTexture) {
		glBindTexture(GL_TEXTURE_2D, m_ImageTexture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_ImageGen->GetWidth(), m_ImageGen->GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, m_ImageGen->GetPixels().data());
		DrawFullscreenQuad(m_ImageTexture, m_Window->GetWidth(), m_Window->GetHeight());
	}

	// ImGui Render
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Menu");
	if (ImGui::Button("Start")) {
		m_ImageGen->Start();
	}
	if (m_ImageGen->IsDone()) {
		ImGui::Text("Image Done in %.3f seconds", m_ImageGen->GetElapsedTime());
		if (ImGui::Button("Save as PNG")) {
			m_ImageGen->SavePNG("image.png");
		}
	}
	else if (m_ImageGen->IsDone() == false && m_ImageGen->GetElapsedTime() == 0.0) {
		ImGui::Text("Waiting...");
	}
	else {
		ImGui::Text("Generating...");
	}
	ImGui::End();

	if (m_ImageGen) {
		m_ImageGen->Update();
	}
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)m_Window->GetWidth(), (float)m_Window->GetHeight());

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

}
void Application::OnKey(int key, int scancode, int action, int mods) {
	// Press ESC to close the window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(m_Window->GetNativeWindow(), true);
}
void Application::OnMouseButton(int button, int action, int mods) {}
void Application::OnCursorPos(double xpos, double ypos) {}
void Application::OnWindowResize(int width, int height) {
	glViewport(0, 0, width, height);
}
void Application::OnFramebufferResize(int width, int height) {
	glViewport(0, 0, width, height);
}
void Application::OnWindowClose() {

	if (m_ImageTexture) {
		glDeleteTextures(1, &m_ImageTexture);
		m_ImageTexture = 0;
	}

	if (g_QuadVAO) glDeleteVertexArrays(1, &g_QuadVAO);
	if (g_QuadVBO) glDeleteBuffers(1, &g_QuadVBO);
	if (g_QuadShader) glDeleteProgram(g_QuadShader);
	g_QuadVAO = g_QuadVBO = g_QuadShader = 0;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
