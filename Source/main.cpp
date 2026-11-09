#include "Glfw/Managers/GlfwWindowManager.h"
#include "Vulkan/Managers/VulkanInstanceManager.h"
#include "Vulkan/Managers/VulkanSurfaceManager.h"
#include "Vulkan/Managers/VulkanPhysicalDeviceManager.h"
#include "Vulkan/Managers/VulkanDeviceManager.h"

int main() {
    GlfwWindowManager::InitCalled initGlfwWindowCalled = GlfwWindowManager::Init();
	VulkanInstanceManager::Init();
	VulkanSurfaceManager::Init(initGlfwWindowCalled);
	VulkanPhysicalDeviceManager::Init();
	VulkanDeviceManager::Init();

	VulkanDeviceManager::Clean();
	VulkanSurfaceManager::Clean();
	VulkanInstanceManager::Clean();
	GlfwWindowManager::Clean();
}