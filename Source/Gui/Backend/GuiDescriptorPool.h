#pragma once

#include <vulkan/vulkan.h>

namespace GuiDescriptorPool {
	extern VkDescriptorPool gDescriptorPool;

	void Init();
	void Clean();
}