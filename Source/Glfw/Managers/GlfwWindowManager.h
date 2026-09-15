#pragma once

#include <GLFW/glfw3.h>
#include "Utility/GeneralUtil.h"

namespace GlfwWindowManager {
	extern GLFWwindow* gGlfwWindow;

	DEFINE_CALLED(Init);

	InitCalled Init();
	void Clean();
}