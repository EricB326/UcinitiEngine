#include "uvkpch.h"
#include "vulkan_context.h"

#include "GLFW/glfw3.h"

namespace Uciniti
{
	vulkan_context::vulkan_context(GLFWwindow* a_window_handle)
		: window_handle(a_window_handle), validation_handle(new vulkan_validation())
	{
		UVK_CORE_ASSERT(window_handle, "Window handle is null!");
	}	

	vulkan_context::~vulkan_context()
	{
		shutdown();
	}

	void vulkan_context::init()
	{
		UVK_CORE_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");

		//glfwMakeContextCurrent(window_handle);

		if (enable_validation_layers)
			UVK_CORE_ASSERT(validation_handle->check_validation_layer_support(), "Validation layers requested, but not available!");
	
		create_instance();

		create_surface();

		select_physical_device();
		create_logical_device();

		create_swap_chain();
	}

	void vulkan_context::begin_frame()
	{
		swap_chain.begin_frame();
	}

	void vulkan_context::swap_buffers()
	{
		swap_chain.present();
	}

	void vulkan_context::create_instance()
	{
		printf("\n");
		UVK_CORE_INFO("Vulkan instance initialisation...");

		// Initialise information about the application.
		VkApplicationInfo application_info(vk_base_application_info);
		application_info.pApplicationName = "Uciniti";
		application_info.pEngineName = "Uciniti";
		application_info.apiVersion = VK_API_VERSION_1_2;

		// Check for extensions and validation.
		std::vector<const char*> instance_extensions = get_instance_extensions();

		if (instance_extensions.size() > 0)
		{
			UVK_CORE_TRACE("Instance extensions required:");
			for (auto this_extension : instance_extensions)
			{
				UVK_CORE_TRACE("\t'{0}'", this_extension);
			}
		}

		instance_extension_support(instance_extensions);

		// Initialise information about the instance.
		VkInstanceCreateInfo instance_info(vk_base_instance_info);
		instance_info.pApplicationInfo = &application_info;
		instance_info.enabledLayerCount = 0;

		// Add the extensions if they are supported.
		instance_info.enabledExtensionCount = static_cast<uint32_t>(instance_extensions.size());
		instance_info.ppEnabledExtensionNames = instance_extensions.data();

		// Add the validation layers if they are enabled.
		VkDebugUtilsMessengerCreateInfoEXT debug_create_info(vk_base_debug_create_info);
		if (enable_validation_layers)
		{
			instance_info.enabledLayerCount = static_cast<uint32_t>(validation_handle->get_validation_layers().size());
			instance_info.ppEnabledLayerNames = validation_handle->get_validation_layers().data();

			validation_handle->populate_debug_messenger_create_info(debug_create_info);
			instance_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;

			UVK_CORE_TRACE("Instance validation layers enabled.");
		}

		VK_CHECK_RESULT(vkCreateInstance(&instance_info, nullptr, &vulkan_instance));
		UVK_CORE_INFO("Vulkan instance created successfully!");

		// Create the validation layers debug callbacks.
		if (enable_validation_layers)
			validation_handle->create();
	}

	void vulkan_context::create_surface()
	{
		swap_chain.create_surface(window_handle, vulkan_instance);
	}

	void vulkan_context::select_physical_device()
	{
		physical_device = vulkan_physical_device::select(swap_chain.get_surface());
	}

	void vulkan_context::create_logical_device()
	{
		VkPhysicalDeviceFeatures device_features = {};
		device_features.samplerAnisotropy = VK_TRUE;

		logical_device = vulkan_logical_device::create(physical_device, device_features);
	}

	void vulkan_context::create_allocator()
	{
		//allocator = vulkan_allocator(logical_device, "Default");
	}

	void vulkan_context::create_swap_chain()
	{
		//swap_chain = new vulkan_swap_chain(window_handle, logical_device);
		//swap_chain.create(window_handle, vulkan_instance, logical_device);
		swap_chain.create_swap_chain(logical_device);
	}

	std::vector<const char*> vulkan_context::get_instance_extensions()
	{
		// Setup extensions that the Vulkan instance will use.
		uint32_t glfw_extension_count = 0; // GLFW may require multiple extensions.
		const char** glfw_extensions;	   // Extensions passed as array of c strings.

		// Get GLFW extensions for the instance.
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		// Create container to return extensions.
		std::vector<const char*> required_extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

		// Push back required extensions for handling debug messages and associated details.
		if (enable_validation_layers)
			required_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return required_extensions;
	}

	bool vulkan_context::instance_extension_support(const std::vector<const char*>& a_required_extensions)
	{
		// Retrieve the number of extensions to create an array of correct size to hold them.
		uint32_t extension_count = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

		// Create a container of VkExtensionProperties using the extension count.
		std::vector<VkExtensionProperties> extensions(extension_count);
		vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

		// Loop over each required extensions, and the extensions the instance has available.
		// If the required extensions is not within the available extensions, then the program
		// should not run.
		for (const auto& this_required_extension : a_required_extensions)
		{
			bool required_extension_available = false;

			for (const auto& this_available_extension : extensions)
			{
				if (strcmp(this_required_extension, this_available_extension.extensionName) == 0)
				{
					required_extension_available = true;
					break;
				}
			}

			if (!required_extension_available)
			{
				std::string assert_msg = "The extension '" + std::string(this_required_extension) + "' is not available!";
				UVK_CORE_ASSERT(!required_extension_available, assert_msg);
				return false;
			}
		}

		UVK_CORE_TRACE("All extensions successfully found.");
		return true;
	}

	void vulkan_context::shutdown()
	{
		swap_chain.shutdown();

		logical_device->shutdown();

		if (enable_validation_layers)
			validation_handle.reset(nullptr);

		vkDestroyInstance(vulkan_instance, nullptr);
	}

}