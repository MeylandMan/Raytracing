#pragma once

#include "Window.h"
#include <memory>

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

	void RegisterCallbacks();

	std::unique_ptr<Window> m_Window;
};
