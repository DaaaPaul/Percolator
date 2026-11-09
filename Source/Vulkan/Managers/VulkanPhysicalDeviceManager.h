#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace VulkanPhysicalDeviceManager {
	extern VkPhysicalDevice gPhysicalDevice;
	extern VkPhysicalDeviceProperties gPhysicalDeviceProperties;
	extern VkPhysicalDeviceLimits gPhysicalDeviceLimits;
	extern uint32_t gGraphicsQueueFamilyIndex;

	void Init();
	void PrintInfo();
}