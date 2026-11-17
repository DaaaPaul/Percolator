#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "Glfw/Managers/GlfwWindowManager.h"

namespace VulkanSwapchainManager {
	extern VkSwapchainKHR gSwapchain;
	extern std::vector<VkImage> gImages;
	extern std::vector<VkImageView> gImageViews;

	extern VkSurfaceCapabilitiesKHR gSurfaceCapabilities;

	constexpr VkFormat gIMAGE_FORMAT = VK_FORMAT_B8G8R8A8_SRGB;
	constexpr VkColorSpaceKHR gIMAGE_COLORSPACE = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	constexpr VkPresentModeKHR gPRESENT_MODE = VK_PRESENT_MODE_MAILBOX_KHR;
	constexpr uint32_t gIMAGE_COUNT = 2;
	extern VkExtent2D gImageExtent;

	void Init();
	void Clean();
}