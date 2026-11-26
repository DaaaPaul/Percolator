#pragma once

#include <vulkan/vulkan.h>

namespace GuiCommandBuffer {
	extern VkCommandPool gCommandPool;
	extern VkCommandBuffer gCommandBuffer;
	constexpr uint32_t gCOMMAND_BUFFER_COUNT = 1;

	void Init();
	void Clean();
}