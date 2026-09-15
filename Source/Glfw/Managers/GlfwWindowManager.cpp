#include <cassert>
#include "GlfwWindowManager.h"

namespace GlfwWindowManager {
	GLFWwindow* gGlfwWindow = nullptr;

	static void InitGlfwState() {
		glfwInit();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	static void SetGlfwCallbacks() {
	
	}

	InitCalled Init() {
		InitGlfwState();

		gGlfwWindow = glfwCreateWindow(800, 600, "Percolator", nullptr, nullptr);
		if(!gGlfwWindow) {
			assert(false && "glfwCreateWindow returned nullptr");
		}

		SetGlfwCallbacks();

		return {};
	}

	void Clean() {
		glfwDestroyWindow(gGlfwWindow);
		glfwTerminate();
	}
}