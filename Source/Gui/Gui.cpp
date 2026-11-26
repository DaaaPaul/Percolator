#include "Gui.h"
#include "Glfw/Managers/GlfwWindowManager.h"
#include "Vulkan/Managers/VulkanInstanceManager.h"
#include "Vulkan/Managers/VulkanDeviceManager.h"
#include "Vulkan/Managers/VulkanPhysicalDeviceManager.h"
#include "Vulkan/Managers/VulkanSwapchainManager.h"
#include "Backend/GuiDescriptorPool.h"
#include "Utility/VulkanUtil.h"
#include "Utility/GeneralUtil.h"

namespace Gui {
	ImGui_ImplVulkan_InitInfo gVulkanInitInfo{};

	static void ImGuiCheckFn(VkResult result) {
		VK_CHECK(result)
	}

	static void InitImGuiContext() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
	}

	static void InitImGuiIOContext() {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(VulkanSwapchainManager::gImageExtent.width);
		io.DisplaySize.y = static_cast<float>(VulkanSwapchainManager::gImageExtent.height);
		io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableSetMousePos;
		io.BackendFlags = 0;

		ImGui_ImplGlfw_InitForVulkan(GlfwWindowManager::gGlfwWindow, true);
	}

	static void InitVulkanInitInfo() {
		gVulkanInitInfo.ApiVersion = VulkanInstanceManager::gVK_API_VERSION;
		gVulkanInitInfo.Instance = VulkanInstanceManager::gInstance;
		gVulkanInitInfo.PhysicalDevice = VulkanPhysicalDeviceManager::gPhysicalDevice;
		gVulkanInitInfo.Device = gDevice;
		gVulkanInitInfo.QueueFamily = VulkanPhysicalDeviceManager::gGraphicsQueueFamilyIndex;
		gVulkanInitInfo.Queue = VulkanDeviceManager::gGraphicsQueue;
		gVulkanInitInfo.DescriptorPool = GuiDescriptorPool::gDescriptorPool;
		gVulkanInitInfo.MinImageCount = VulkanSwapchainManager::gSurfaceCapabilities.minImageCount;
		gVulkanInitInfo.ImageCount = VulkanSwapchainManager::gIMAGE_COUNT;
		gVulkanInitInfo.PipelineInfoMain = ImGui_ImplVulkan_PipelineInfo{
			.MSAASamples = VK_SAMPLE_COUNT_1_BIT,
			.PipelineRenderingCreateInfo = VkPipelineRenderingCreateInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
				.colorAttachmentCount = 1,
				.pColorAttachmentFormats = &VulkanSwapchainManager::gIMAGE_FORMAT,
				.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT
			}
		};
		gVulkanInitInfo.UseDynamicRendering = true;
		gVulkanInitInfo.CheckVkResultFn = &ImGuiCheckFn;

		ImGui_ImplVulkan_Init(&gVulkanInitInfo);
	}

	DEFINE_CALLED(RecordFrame)

	RecordFrameCalled RecordFrame() {
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();

		return {};
	}

	void Init() {
		InitImGuiContext();
		InitImGuiIOContext();
		InitVulkanInitInfo();
	}

	void Clean() {
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}