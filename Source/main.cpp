#include <iostream>
#include "Glfw/Managers/GlfwWindowManager.h"
#include "Vulkan/Managers/VulkanInstanceManager.h"
#include "Vulkan/Managers/VulkanSurfaceManager.h"
#include "Vulkan/Managers/VulkanPhysicalDeviceManager.h"
#include "Vulkan/Managers/VulkanDeviceManager.h"
#include "Vulkan/Managers/VulkanSwapchainManager.h"
#include "Gui/Backend/GuiDescriptorPool.h"

int main() {
    GlfwWindowManager::InitCalled initGlfwWindowCalled = GlfwWindowManager::Init();
	VulkanInstanceManager::Init();
	VulkanSurfaceManager::Init(initGlfwWindowCalled);
	VulkanPhysicalDeviceManager::Init();
	VulkanDeviceManager::Init();
	VulkanSwapchainManager::Init();

	GuiDescriptorPool::Init();

	GuiDescriptorPool::Clean();

	VulkanSwapchainManager::Clean();
	VulkanDeviceManager::Clean();
	VulkanSurfaceManager::Clean();
	VulkanInstanceManager::Clean();
	GlfwWindowManager::Clean();
}