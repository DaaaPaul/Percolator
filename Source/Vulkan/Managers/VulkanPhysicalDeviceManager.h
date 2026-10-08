#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace VulkanPhysicalDeviceManager {
	extern VkPhysicalDevice gPhysicalDevice;
	extern VkPhysicalDeviceLimits gPhysicalDeviceLimits;
	extern uint32_t gGraphicsQueueFamilyIndex;

	void Init(const std::vector<const char*>* pREQUIRED_EXTENSIONS);
	void Clean();
}