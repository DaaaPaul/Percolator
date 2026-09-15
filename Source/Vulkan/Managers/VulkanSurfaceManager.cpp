#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Utility/VulkanUtil.h"
#include "VulkanInstanceManager.h"
#include "VulkanSurfaceManager.h"

namespace VulkanSurfaceManager {
	VkSurfaceKHR gSurface = VK_NULL_HANDLE;

	void Init(GlfwWindowManager::InitCalled initGlfwWindowCalled) {
		VK_CHECK(glfwCreateWindowSurface(VulkanInstanceManager::gInstance, GlfwWindowManager::gGlfwWindow, nullptr, &gSurface))
	}

	void Clean() {
		vkDestroySurfaceKHR(VulkanInstanceManager::gInstance, gSurface, nullptr);
	}
}