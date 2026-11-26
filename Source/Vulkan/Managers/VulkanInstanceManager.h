#pragma once

#include <vulkan/vulkan.h>

namespace VulkanInstanceManager {
	extern VkInstance gInstance;
	extern VkDebugUtilsMessengerEXT gDebugMessenger;
	constexpr uint32_t gVK_API_VERSION = VK_API_VERSION_1_3;

	void Init();
	void Clean();
}