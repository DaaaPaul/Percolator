#pragma once

#include <vulkan/vulkan.h>

namespace VulkanDeviceManager {
	extern VkDevice gDevice;
	extern VkQueue gGraphicsQueue;
	constexpr uint32_t gQUEUE_INDEX = 0;

	void Init();
	void Clean();
}

using VulkanDeviceManager::gDevice;