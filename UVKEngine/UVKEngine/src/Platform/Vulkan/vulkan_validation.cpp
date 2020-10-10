#include "uvkpch.h"
#include "vulkan_validation.h"

#include "vulkan_context.h"

namespace Uciniti
{
	vulkan_validation::vulkan_validation()
		: validation_layers({"VK_LAYER_KHRONOS_validation"})
	{

	}

	vulkan_validation::~vulkan_validation()
	{
		shutdown();
	}

	void vulkan_validation::populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& a_debug_create_info)
	{
		a_debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		a_debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		a_debug_create_info.pfnUserCallback = &debug_callback;
	}

	bool vulkan_validation::check_validation_layer_support()
	{
		// Retrieve the number of inst layer properties to create an array of the correct size with.
		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		// Create a container of VkLayerProperties using the layer count.
		std::vector<VkLayerProperties> available_layers(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

		for (const auto& this_layer : validation_layers)
		{
			bool required_layer_found = false;

			for (const auto& this_available_layer : available_layers)
			{
				if (strcmp(this_layer, this_available_layer.layerName) == 0)
				{
					required_layer_found = true;
					break;
				}
			}

			if (!required_layer_found)
			{
				std::string assert_msg = "The layer '" + std::string(this_layer) + "' is not available!";
				UVK_CORE_ASSERT(!required_layer_found, assert_msg);
				return false;
			}
		}

		return true;
	}

	void vulkan_validation::create()
	{
		if (!enable_validation_layers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT debug_create_info(vk_base_debug_create_info);
		populate_debug_messenger_create_info(debug_create_info);

		VK_CHECK_RESULT(create_debug_utils_messenger(&debug_create_info));
	}

	void vulkan_validation::shutdown()
	{
		if (enable_validation_layers)
			destroy_debug_utils_messenger();
	}

	VkResult vulkan_validation::create_debug_utils_messenger(const VkDebugUtilsMessengerCreateInfoEXT* a_create_info)
	{
		auto instance_handle = vulkan_context::get_instance();

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_handle, "vkCreateDebugUtilsMessengerEXT");

		if (func)
			return func(instance_handle, a_create_info, nullptr, &debug_messenger);
		else
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void vulkan_validation::destroy_debug_utils_messenger()
	{
		auto instance_handle = vulkan_context::get_instance();

		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_handle, "vkDestroyDebugUtilsMessengerEXT");

		if (func)
			func(instance_handle, debug_messenger, nullptr);
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_validation::debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT a_message_severity, VkDebugUtilsMessageTypeFlagsEXT a_message_type, const VkDebugUtilsMessengerCallbackDataEXT* a_callback_data, void* a_user_data)
	{
		switch (a_message_severity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			//UVK_CORE_INFO("{0}", a_callback_data->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			printf("\n");
			UVK_CORE_WARN("{0}", a_callback_data->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			printf("\n");
			UVK_CORE_ERROR("{0}", a_callback_data->pMessage);
			break;
		default:
			UVK_CORE_INFO("Undefined");
			break;
		}

		return VK_FALSE;
	}
}