#pragma once

#include <vulkan/vulkan.h>
#include <cassert>

#define VK_CHECK(result) \
	if(result != VK_SUCCESS) { \
		assert(false && #result); \
	}