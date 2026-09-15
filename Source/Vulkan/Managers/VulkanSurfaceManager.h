#pragma once

#include <vulkan/vulkan.h>
#include "Glfw/Managers/GlfwWindowManager.h"

namespace VulkanSurfaceManager {
	extern VkSurfaceKHR gSurface;

	void Init(GlfwWindowManager::InitCalled initGlfwWindowCalled);
	void Clean();
}