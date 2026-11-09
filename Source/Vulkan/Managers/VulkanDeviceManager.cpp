#include <vulkan/vulkan.hpp>
#include <cassert>
#include <cstring>
#include <vector>

#include "VulkanDeviceManager.h"
#include "VulkanPhysicalDeviceManager.h"
#include "Utility/VulkanUtil.h"

namespace VulkanDeviceManager {
	VkDevice gDevice = VK_NULL_HANDLE;
	VkQueue gGraphicsQueue = VK_NULL_HANDLE;

	void Init() {
		assert(gDevice == VK_NULL_HANDLE);
		assert(VulkanPhysicalDeviceManager::gPhysicalDevice != VK_NULL_HANDLE);
		assert(VulkanPhysicalDeviceManager::gGraphicsQueueFamilyIndex != UINT32_MAX);

		const float QUEUE_PRIORITY = 1.0f;

		VkDeviceQueueCreateInfo queueCreateInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = VulkanPhysicalDeviceManager::gGraphicsQueueFamilyIndex,
			.queueCount = 1,
			.pQueuePriorities = &QUEUE_PRIORITY
		};

		std::vector<const char*> requiredExtensions{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
			VK_KHR_SPIRV_1_4_EXTENSION_NAME,
			VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME,
			VK_KHR_UNIFIED_IMAGE_LAYOUTS_EXTENSION_NAME,
			VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME
		};

		uint32_t extensionCount{};
		VK_CHECK(vkEnumerateDeviceExtensionProperties(VulkanPhysicalDeviceManager::gPhysicalDevice, nullptr, &extensionCount, nullptr));
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		VK_CHECK(vkEnumerateDeviceExtensionProperties(VulkanPhysicalDeviceManager::gPhysicalDevice,nullptr, &extensionCount, availableExtensions.data()));

		for(const VkExtensionProperties& EXTENSION : availableExtensions) {
			if(std::strcmp(EXTENSION.extensionName, "VK_KHR_portability_subset") == 0) {
				requiredExtensions.push_back("VK_KHR_portability_subset");
				break;
			}
		}

		vk::StructureChain<
			vk::PhysicalDeviceFeatures2,
			vk::PhysicalDeviceBufferDeviceAddressFeatures,
			vk::PhysicalDeviceTimelineSemaphoreFeatures,
			vk::PhysicalDeviceSynchronization2Features,
			vk::PhysicalDeviceDynamicRenderingFeatures,
			vk::PhysicalDeviceExtendedDynamicState2FeaturesEXT,
			vk::PhysicalDeviceHostImageCopyFeaturesEXT,
			vk::PhysicalDeviceUnifiedImageLayoutsFeaturesKHR
		> features{};

		features.get<vk::PhysicalDeviceFeatures2>().features.samplerAnisotropy = VK_TRUE;
		features.get<vk::PhysicalDeviceFeatures2>().features.textureCompressionBC = VK_TRUE;
		features.get<vk::PhysicalDeviceBufferDeviceAddressFeatures>().bufferDeviceAddress = VK_TRUE;
		features.get<vk::PhysicalDeviceTimelineSemaphoreFeatures>().timelineSemaphore = VK_TRUE;
		features.get<vk::PhysicalDeviceSynchronization2Features>().synchronization2 = VK_TRUE;
		features.get<vk::PhysicalDeviceDynamicRenderingFeatures>().dynamicRendering = VK_TRUE;
		features.get<vk::PhysicalDeviceExtendedDynamicState2FeaturesEXT>().extendedDynamicState2 = VK_TRUE;
		features.get<vk::PhysicalDeviceHostImageCopyFeaturesEXT>().hostImageCopy = VK_TRUE;
		features.get<vk::PhysicalDeviceUnifiedImageLayoutsFeaturesKHR>().unifiedImageLayouts = VK_TRUE;

		VkDeviceCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = &*features.get<vk::PhysicalDeviceFeatures2>(),
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &queueCreateInfo,
			.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
			.ppEnabledExtensionNames = requiredExtensions.data()
		};

		VK_CHECK(vkCreateDevice(VulkanPhysicalDeviceManager::gPhysicalDevice, &createInfo, nullptr, &gDevice));
		vkGetDeviceQueue(gDevice, VulkanPhysicalDeviceManager::gGraphicsQueueFamilyIndex, 0, &gGraphicsQueue);
	}

	void Clean() {
		if(gDevice == VK_NULL_HANDLE) {
			return;
		}

		VK_CHECK(vkDeviceWaitIdle(gDevice));

		vkDestroyDevice(gDevice, nullptr);

		gGraphicsQueue = VK_NULL_HANDLE;
		gDevice = VK_NULL_HANDLE;
	}
}