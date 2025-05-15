#include "Application.h"

#include <iostream>

#include <misc/cpp/imgui_stdlib.cpp>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_glfw.cpp>

#include <GLFW/glfw3.h>

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

	// ImGui Render
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Menu");

	if (ImGui::Button("Start"))
	{
		std::cout << "Start button pressed!" << std::endl;
	}

	ImGui::End();
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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
