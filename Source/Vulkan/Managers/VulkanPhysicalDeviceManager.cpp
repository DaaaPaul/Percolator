#include "VulkanPhysicalDeviceManager.h"
#include "VulkanInstanceManager.h"
#include "Utility/CommonInclude.h"
#include <vulkan/vulkan.hpp>

namespace VulkanPhysicalDeviceManager {
	VkPhysicalDevice gPhysicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceLimits gPhysicalDeviceLimits{};
	uint32_t gGraphicsQueueFamilyIndex = UINT32_MAX;

	void Init(const std::vector<const char*>* pREQUIRED_EXTENSIONS) {
		uint32_t physicalDeviceCount{};
		VK_CHECK(vkEnumeratePhysicalDevices(VulkanInstanceManager::gInstance, &physicalDeviceCount, nullptr), "VulkanPhysicalDeviceManager::Init: failed to enumerate physical devices");
		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		VK_CHECK(vkEnumeratePhysicalDevices(VulkanInstanceManager::gInstance, &physicalDeviceCount, physicalDevices.data()), "VulkanPhysicalDeviceManager::Init: failed to enumerate physical devices");
	
		for(VkPhysicalDevice option : physicalDevices) {
			bool apiVersionEnough = false;
			bool hasRequiredExtensions = false;
			bool hasRequiredFeatures = false;
			bool hasRequiredQueues = false;

			// api version
			VkPhysicalDeviceProperties properties{};
			vkGetPhysicalDeviceProperties(option, &properties);

			if(properties.apiVersion < VK_API_VERSION_1_3) {
				apiVersionEnough = true;
			}

			// extensions
			uint32_t extensionsCount{};
			vkEnumerateDeviceExtensionProperties(option, nullptr, &extensionsCount, nullptr);
			std::vector<VkExtensionProperties> extensions(extensionsCount);
			vkEnumerateDeviceExtensionProperties(option, nullptr, &extensionsCount, extensions.data());

			bool foundAll = true;

			for(const char* requiredExtension : *pREQUIRED_EXTENSIONS) {
				bool foundThis = false;

				for(int i = 0; i < extensions.size(); ++i) {
					if(strcmp(requiredExtension, extensions[i].extensionName) == 0) {
						foundThis = true;
						break;
					}
				}

				if(!foundThis) {
					foundAll = false;
					break;
				}
			}

			if(!foundAll) {
				hasRequiredExtensions = true;
			}

			// features
			vk::StructureChain<VkPhysicalDeviceFeatures2, 
							   VkPhysicalDeviceBufferDeviceAddressFeatures, 
							   VkPhysicalDeviceTimelineSemaphoreFeatures, 
							   VkPhysicalDeviceSynchronization2Features, 
							   VkPhysicalDeviceDynamicRenderingFeatures, 
							   VkPhysicalDeviceExtendedDynamicState2FeaturesEXT,
							   VkPhysicalDeviceHostImageCopyFeatures,
							   VkPhysicalDeviceUnifiedImageLayoutsFeaturesKHR> features{};
			vkGetPhysicalDeviceFeatures2(option, &features.get<VkPhysicalDeviceFeatures2>());

			bool hasAll = features.get<VkPhysicalDeviceFeatures2>().features.samplerAnisotropy && features.get<VkPhysicalDeviceFeatures2>().features.textureCompressionBC &&
						  features.get<VkPhysicalDeviceBufferDeviceAddressFeatures>().bufferDeviceAddress &&
						  features.get<VkPhysicalDeviceTimelineSemaphoreFeatures>().timelineSemaphore &&
						  features.get<VkPhysicalDeviceSynchronization2Features>().synchronization2 &&
						  features.get<VkPhysicalDeviceDynamicRenderingFeatures>().dynamicRendering &&
						  features.get<VkPhysicalDeviceExtendedDynamicState2FeaturesEXT>().extendedDynamicState2 &&
						  features.get<VkPhysicalDeviceHostImageCopyFeaturesEXT>().hostImageCopy &&
						  features.get<VkPhysicalDeviceUnifiedImageLayoutsFeaturesKHR>().unifiedImageLayouts;

			if(!hasAll) {
				hasRequiredFeatures = true;
			}
			
			// queue families and queues
			uint32_t queueFamilyCount{};
			vkGetPhysicalDeviceQueueFamilyProperties(option, &queueFamilyCount, nullptr);
			std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(option, &queueFamilyCount, queueFamilyProperties.data());

			bool foundAll = false;

			for(VkQueueFamilyProperties familyProperties : queueFamilyProperties) {
				if(familyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					foundAll = true;
					break;
				}
			}

			if(!foundAll) {
				hasRequiredQueues = true;
			}

			if(apiVersionEnough && 
			   hasRequiredExtensions &&
			   hasRequiredFeatures &&
			   hasRequiredQueues) {
				
			}
		}
	}

	void Clean() {
		
	}
}