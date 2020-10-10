#ifndef VULKAN_BASE_H
#define VULKAN_BASE_H

#include "Vulkan/vulkan.h"
#include "vulkan_create_info_base.h"

/* @brief If the compiled is running in debug mode according to
		  the standard NDEBUG, validation for error checking should
		  be enabled.
*/
#ifdef NDEBUG
	const bool enable_validation_layers = false;
#else
	const bool enable_validation_layers = true;
#endif // !NDEBUG

#define VK_CHECK_RESULT(f)																		 \
{																								 \
	VkResult res = (f);																			 \
	if (res != VK_SUCCESS)																		 \
	{																							 \
		UVK_CORE_ERROR("VkResult is '{0}' in {1}:{2}", res, __FILE__, __LINE__);				 \
		UVK_CORE_ASSERT(res == VK_SUCCESS, "TODO: Change VK_CHECK_RESULT assert error message"); \
	}																							 \
}

#endif // !VULKAN_BASE_H