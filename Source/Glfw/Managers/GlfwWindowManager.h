#pragma once

#include <GLFW/glfw3.h>
#include "Utility/GeneralUtil.h"

namespace GlfwWindowManager {
	extern GLFWwindow* gGlfwWindow;
	constexpr int gWINDOW_WIDTH = 800;
	constexpr int gWINDOW_HEIGHT = 600;

	DEFINE_CALLED(Init);

	InitCalled Init();
	void Clean();
}