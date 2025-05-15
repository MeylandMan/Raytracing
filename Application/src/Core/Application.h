#pragma once

#include "Image.h"
#include "Window.h"

#include <memory>

#include <imgui.h>
#include <imgui_internal.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class Application
{
public:
	Application(int width, int height, const std::string& title);
	~Application();

	void Run();

public:
	void OnUpdate();
	void OnKey(int key, int scancode, int action, int mods);
	void OnMouseButton(int button, int action, int mods);
	void OnCursorPos(double xpos, double ypos);
	void OnWindowResize(int width, int height);
	void OnFramebufferResize(int width, int height);
	void OnWindowClose();

protected:
	void SetDarkThemeColors();
	void RegisterCallbacks();

	std::unique_ptr<Window> m_Window;
	std::unique_ptr<Image> m_ImageGen;

	unsigned int m_ImageTexture = 0;
};
