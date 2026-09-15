#include "Glfw/Managers/GlfwWindowManager.h"
#include "Vulkan/Managers/VulkanInstanceManager.h"
#include "Vulkan/Managers/VulkanSurfaceManager.h"

int main() {
    GlfwWindowManager::InitCalled initGlfwWindowCalled = GlfwWindowManager::Init();
	VulkanInstanceManager::Init();
	VulkanSurfaceManager::Init(initGlfwWindowCalled);

	VulkanSurfaceManager::Clean();
	VulkanInstanceManager::Clean();
	GlfwWindowManager::Clean();
}