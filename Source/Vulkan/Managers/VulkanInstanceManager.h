#pragma once

#include <vulkan/vulkan.h>

namespace VulkanInstanceManager {
	extern VkInstance gInstance;
	extern VkDebugUtilsMessengerEXT gDebugMessenger;

	void Init();
	void Clean();
}