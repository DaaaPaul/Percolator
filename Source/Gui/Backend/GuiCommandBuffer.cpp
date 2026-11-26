#include "GuiCommandBuffer.h"
#include "Vulkan/Managers/VulkanPhysicalDeviceManager.h"
#include "Vulkan/Managers/VulkanDeviceManager.h"
#include "Utility/VulkanUtil.h"

namespace GuiCommandBuffer {
	VkCommandPool gCommandPool = VK_NULL_HANDLE;
	VkCommandBuffer gCommandBuffer = VK_NULL_HANDLE;

	static void InitCommandPool() {
		VkCommandPoolCreateInfo commandPoolCreate{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = VulkanPhysicalDeviceManager::gGraphicsQueueFamilyIndex
		};

		VK_CHECK(vkCreateCommandPool(gDevice, &commandPoolCreate, nullptr, &gCommandPool))
	}

	static void InitCommandBuffer() {
		VkCommandBufferAllocateInfo commandBufferCreate{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = gCommandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = gCOMMAND_BUFFER_COUNT
		};

		VK_CHECK(vkAllocateCommandBuffers(gDevice, &commandBufferCreate, &gCommandBuffer))
	}

	void Init() {
		InitCommandPool();
		InitCommandBuffer();
	}

	void Clean() {
		vkFreeCommandBuffers(gDevice, gCommandPool, gCOMMAND_BUFFER_COUNT, &gCommandBuffer);
		vkDestroyCommandPool(gDevice, gCommandPool, nullptr);
	}
}