#pragma once

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_vulkan.h"

namespace Gui {
	extern ImGui_ImplVulkan_InitInfo gVulkanInitInfo;

	void Init();
	void Clean();
}