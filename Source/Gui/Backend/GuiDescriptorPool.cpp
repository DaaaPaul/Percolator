#include <vulkan/vulkan.h>
#include <vector>
#include "GuiDescriptorPool.h"
#include "Utility/VulkanUtil.h"
#include "Vulkan/Managers/VulkanDeviceManager.h"

namespace GuiDescriptorPool {
	VkDescriptorPool gDescriptorPool = VK_NULL_HANDLE;

	void Init() {
		constexpr uint32_t SET_NUM = 1000;

		std::vector<VkDescriptorPoolSize> poolSizes{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, SET_NUM },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, SET_NUM }
		};

		const uint32_t TYPE_NUM = poolSizes.size();

		VkDescriptorPoolCreateInfo descriptorPoolCreate = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
			.maxSets = SET_NUM * TYPE_NUM,
			.poolSizeCount = TYPE_NUM,
			.pPoolSizes = poolSizes.data()
		};

		VK_CHECK(vkCreateDescriptorPool(gDevice, &descriptorPoolCreate, nullptr, &gDescriptorPool))
	}

	void Clean() {
		vkDestroyDescriptorPool(gDevice, gDescriptorPool, nullptr);
	}
}