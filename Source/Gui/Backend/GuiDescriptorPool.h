#pragma once

#include <vulkan/vulkan.h>

namespace GuiDescriptorPool {
	extern VkDescriptorPool descriptorPool;

	void Init();
	void Clean();
}