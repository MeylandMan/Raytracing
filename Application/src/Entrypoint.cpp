#include <fstream>

#include "Core/Application.h"

int main(int argc, char** argv) {

	Application* app = new Application(800, 600, "RayTracing in one Weekend");
	app->Run();

	delete app;
	return 0;

}
