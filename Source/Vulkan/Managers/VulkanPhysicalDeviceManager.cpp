#include <vulkan/vulkan.hpp>
#include "VulkanPhysicalDeviceManager.h"
#include "VulkanInstanceManager.h"
#include "Utility/CommonInclude.h"

namespace VulkanPhysicalDeviceManager {
	VkPhysicalDevice gPhysicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceLimits gPhysicalDeviceLimits{};
	uint32_t gGraphicsQueueFamilyIndex = UINT32_MAX;

	void Init() {
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

			bool foundAllExtensions = true;

			std::vector<const char*> requiredExtensions{
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
				VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
				VK_KHR_SPIRV_1_4_EXTENSION_NAME,
				VK_EXT_HOST_IMAGE_COPY_EXTENSION_NAME,
				VK_KHR_UNIFIED_IMAGE_LAYOUTS_EXTENSION_NAME,
				VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
				#ifdef __APPLE__
				VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
				#endif
			};

			for(const char* requiredExtension : requiredExtensions) {
				bool foundThis = false;

				for(int i = 0; i < extensions.size(); ++i) {
					if(strcmp(requiredExtension, extensions[i].extensionName) == 0) {
						foundThis = true;
						break;
					}
				}

				if(!foundThis) {
					foundAllExtensions = false;
					break;
				}
			}

			if(!foundAllExtensions) {
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

			bool foundAllQueueFamilies = false;

			for(VkQueueFamilyProperties familyProperties : queueFamilyProperties) {
				if(familyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					foundAllQueueFamilies = true;
					break;
				}
			}

			if(!foundAllQueueFamilies) {
				hasRequiredQueues = true;
			}

			if(apiVersionEnough && 
			    hasRequiredExtensions &&
			    hasRequiredFeatures &&
			    hasRequiredQueues) {
				gPhysicalDevice = option;

				VkPhysicalDeviceProperties properties{};
				vkGetPhysicalDeviceProperties(option, &properties);

				gPhysicalDeviceLimits = properties.limits;

				uint32_t queueFamilyCount{};
				vkGetPhysicalDeviceQueueFamilyProperties(option, &queueFamilyCount, nullptr);
				std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(option, &queueFamilyCount, queueFamilyProperties.data());

				for(int i = 0; i < queueFamilyProperties.size(); ++i) {
					if(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
						gGraphicsQueueFamilyIndex = i;
						break;
					}
				}
			}
		}

		if(gPhysicalDevice == VK_NULL_HANDLE) {
			assert(false && "Did not find valid physical device");
		}
	}
}