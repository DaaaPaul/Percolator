#include <GLFW/glfw3.h>
#include <vector>
#include <cassert>
#include <iostream>
#include "Utility/VulkanUtil.h"
#include "Utility/GeneralUtil.h"
#include "VulkanInstanceManager.h"

namespace VulkanInstanceManager {
	VkInstance gInstance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT gDebugMessenger = VK_NULL_HANDLE;

	DEFINE_CALLED(InitInstance)

	static InitInstanceCalled InitInstance() {
		const std::vector<const char*> INSTANCE_LAYERS{ "VK_LAYER_KHRONOS_validation" };

		{
			uint32_t count{};
			vkEnumerateInstanceLayerProperties(&count, nullptr);
			std::vector<VkLayerProperties> layers(count);
			vkEnumerateInstanceLayerProperties(&count, layers.data());

			bool found = false;

			for(int i = 0; i < layers.size() && !found; ++i) {
				if(strcmp(layers[i].layerName, "VK_LAYER_KHRONOS_validation") == 0) {
					found = true;
				}
			}

			if(!found) {
				assert(false && "VK_LAYER_KHRONOS_validation not found");
			}
		}

		std::vector<const char*> instanceExtensions{ VK_EXT_DEBUG_UTILS_EXTENSION_NAME };

		{
			glfwInit();

			uint32_t count{};
			const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);

			for(int i = 0; i < count; ++i) {
				instanceExtensions.push_back(glfwExtensions[i]);
			}

			#ifdef __APPLE__
			instanceExtensions.push_back("VK_KHR_portability_enumeration");
			instanceExtensions.push_back("VK_KHR_surface");
			instanceExtensions.push_back("VK_EXT_metal_surface");
			#endif
		}

		{
			bool foundAll = true;

			uint32_t count{};
			vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
			std::vector<VkExtensionProperties> extensions(count);
			vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

			bool foundThis = false;

			for(int i = 0; i < instanceExtensions.size(); ++i) {
				bool foundThis = false;

				for(int j = 0; j < extensions.size(); ++j) {
					if(strcmp(instanceExtensions[i], extensions[j].extensionName) == 0) {
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
				assert(false && "Not all required instance extensions are available");
			}
		}

		constexpr VkApplicationInfo APP_INFO{
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.apiVersion = VK_API_VERSION_1_3
		};
		const VkInstanceCreateInfo CREATE_INFO{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			#ifdef __APPLE__
				.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
			#endif
			.pApplicationInfo = &APP_INFO,
			.enabledLayerCount = static_cast<uint32_t>(INSTANCE_LAYERS.size()),
			.ppEnabledLayerNames = INSTANCE_LAYERS.data(),
			.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size()),
			.ppEnabledExtensionNames = instanceExtensions.data()
		};

		VK_CHECK(vkCreateInstance(&CREATE_INFO, nullptr, &gInstance))

		return {};
	}

    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCALLBACK_DATA, void* pUserData) {
		std::cout << "DebugMessengerCallback: " << pCALLBACK_DATA->pMessage << '\n';

        return {};
    }

	static void InitDebugMessenger(InitInstanceCalled initInstanceCalled) {
		constexpr VkDebugUtilsMessengerCreateInfoEXT CREATE_INFO{
			.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			.pfnUserCallback = &DebugMessengerCallback
		};

		auto vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(gInstance, "vkCreateDebugUtilsMessengerEXT"));
		VK_CHECK(vkCreateDebugUtilsMessengerEXT(gInstance, &CREATE_INFO, nullptr, &gDebugMessenger))
	}

	void Init() {
		InitDebugMessenger(InitInstance());
	}

	void Clean() {
		auto vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(gInstance, "vkDestroyDebugUtilsMessengerEXT"));
		vkDestroyDebugUtilsMessengerEXT(gInstance, gDebugMessenger, nullptr);

		vkDestroyInstance(gInstance, nullptr);
	}
}