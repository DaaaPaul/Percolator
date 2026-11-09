#pragma once

#include <vulkan/vulkan.h>

namespace VulkanDeviceManager {
	extern VkDevice gDevice;
	extern VkQueue gGraphicsQueue;

	void Init();
	void Clean();
}

using VulkanDeviceManager::gDevice;