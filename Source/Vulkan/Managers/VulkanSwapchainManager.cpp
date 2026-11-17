#include "VulkanSwapchainManager.h"
#include "VulkanDeviceManager.h"
#include "VulkanPhysicalDeviceManager.h"
#include "VulkanSurfaceManager.h"
#include "Utility/VulkanUtil.h"

#include <algorithm>
#include <cassert>

namespace VulkanSwapchainManager {
	VkSwapchainKHR gSwapchain = VK_NULL_HANDLE;
	std::vector<VkImage> gImages{};
	std::vector<VkImageView> gImageViews{};

	VkSurfaceCapabilitiesKHR gSurfaceCapabilities{};

	VkExtent2D gImageExtent{};

	bool Recreate() {
		VkPhysicalDevice physicalDevice = VulkanPhysicalDeviceManager::gPhysicalDevice;
		VkSurfaceKHR surface = VulkanSurfaceManager::gSurface;
		uint32_t queueFamily = VulkanPhysicalDeviceManager::gGraphicsQueueFamilyIndex;

		assert(gDevice != VK_NULL_HANDLE);
		assert(physicalDevice != VK_NULL_HANDLE);
		assert(surface != VK_NULL_HANDLE);
		assert(queueFamily != UINT32_MAX);
		assert(GlfwWindowManager::gGlfwWindow != nullptr);

		int width{};
		int height{};
		glfwGetFramebufferSize(GlfwWindowManager::gGlfwWindow, &width, &height);
		if(width <= 0 || height <= 0) {
			return false;
		}

		VkBool32 supportsPresent{};
		VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily, surface, &supportsPresent));
		assert(supportsPresent && "Graphics queue cannot present to the surface");

		VkSurfaceCapabilitiesKHR capabilities{};
		VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

		VkExtent2D extent = capabilities.currentExtent;
		if(extent.width == UINT32_MAX) {
			extent.width = std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			extent.height = std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		}
		if(extent.width <= 0 || extent.height <= 0) {
			return false;
		}

		uint32_t formatCount{};
		VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr));
		std::vector<VkSurfaceFormatKHR> formats(formatCount);
		VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data()));

		bool supportsFormat = false;
		for(VkSurfaceFormatKHR format : formats) {
			if(format.format == gIMAGE_FORMAT && format.colorSpace == gIMAGE_COLORSPACE) {
				supportsFormat = true;
				break;
			}
		}
		assert(supportsFormat && "Surface does not support desired format");

		uint32_t presentModeCount{};
		VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr));
		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()));

		assert(std::find(presentModes.begin(), presentModes.end(), gPRESENT_MODE) != presentModes.end() && "Surface does not support present mode");
		assert(capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		assert(capabilities.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);

		assert((gIMAGE_COUNT >= capabilities.minImageCount) && ((capabilities.maxImageCount == 0) ? true : (gIMAGE_COUNT <= capabilities.maxImageCount)) && "Surface does not support gIMAGE_COUNT images");

		assert(capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);

		if(gSwapchain != VK_NULL_HANDLE) {
			VK_CHECK(vkDeviceWaitIdle(gDevice));
			Clean();
		}

		VkSwapchainCreateInfoKHR createInfo{
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = surface,
			.minImageCount = gIMAGE_COUNT,
			.imageFormat = gIMAGE_FORMAT,
			.imageColorSpace = gIMAGE_COLORSPACE,
			.imageExtent = extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = gPRESENT_MODE,
			.clipped = VK_TRUE
		};
		VK_CHECK(vkCreateSwapchainKHR(gDevice, &createInfo, nullptr, &gSwapchain));

		uint32_t actualImageCount{};
		VK_CHECK(vkGetSwapchainImagesKHR(gDevice, gSwapchain, &actualImageCount, nullptr));
		assert(actualImageCount == gIMAGE_COUNT);

		gImages.resize(gIMAGE_COUNT);
		VK_CHECK(vkGetSwapchainImagesKHR(gDevice, gSwapchain, &actualImageCount, gImages.data()));

		gImageViews.resize(gIMAGE_COUNT);
		for(uint32_t i = 0; i < gIMAGE_COUNT; ++i) {
			VkImageViewCreateInfo viewInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.image = gImages[i],
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = gIMAGE_FORMAT,
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.levelCount = 1,
					.layerCount = 1
				}
			};
			VK_CHECK(vkCreateImageView(gDevice, &viewInfo, nullptr, &gImageViews[i]));
		}

		gSurfaceCapabilities = capabilities;
		gImageExtent = extent;

		return true;
	}

	void Init() {
		assert(gSwapchain == VK_NULL_HANDLE);

		assert(Recreate() && "Cannot initialize swapchain with zero extent");
	}

	void Clean() {
		for(VkImageView view : gImageViews) {
			vkDestroyImageView(gDevice, view, nullptr);
		}
		if(gSwapchain != VK_NULL_HANDLE) {
			vkDestroySwapchainKHR(gDevice, gSwapchain, nullptr);
		}

		gSwapchain = VK_NULL_HANDLE;
		gImages.clear();
		gImageViews.clear();
		gSurfaceCapabilities = {};
		gImageExtent = {};
	}
}