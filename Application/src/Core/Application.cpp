#include "Application.h"

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
	m_Window = std::make_unique<Window>(width, height, title);
	RegisterCallbacks();
	glfwSetWindowUserPointer(m_Window->GetNativeWindow(), this);
}

void Application::RegisterCallbacks() {
	m_Window->SetKeyCallback(KeyCallback);
	m_Window->SetMouseButtonCallback(MouseButtonCallback);
	m_Window->SetCursorPosCallback(CursorPosCallback);
	m_Window->SetWindowSizeCallback(WindowSizeCallback);
	m_Window->SetFramebufferSizeCallback(FramebufferSizeCallback);
	m_Window->SetWindowCloseCallback(WindowCloseCallback);
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

void Application::OnUpdate() {}
void Application::OnKey(int key, int scancode, int action, int mods) {}
void Application::OnMouseButton(int button, int action, int mods) {}
void Application::OnCursorPos(double xpos, double ypos) {}
void Application::OnWindowResize(int width, int height) {
	glViewport(0, 0, width, height);
}
void Application::OnFramebufferResize(int width, int height) {
	glViewport(0, 0, width, height);
}
void Application::OnWindowClose() {}
