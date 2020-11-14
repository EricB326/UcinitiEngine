#ifndef VULKAN_VALIDATION_H
#define VULKAN_VALIDATION_H

#include "vulkan_base.h"

namespace Uciniti
{
	class vulkan_validation
	{
	public:
		/* @brief Constructs the variables to zero.
		*/
		vulkan_validation();

		/* @brief Destroys any memory created or created Vulkan functions.
		*/
		~vulkan_validation();

		void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& a_debug_create_info);
		void create();

		bool check_validation_layer_support();

		inline const std::vector<const char*>& get_validation_layers() const { return validation_layers; }
		inline const VkDebugUtilsMessengerEXT& get_debug_messenger() const { return debug_messenger; }

	private:
		/* @brief Handle for the debug messenger extension.
		*/
		VkDebugUtilsMessengerEXT debug_messenger;

		const std::vector<const char*> validation_layers;

		void shutdown();

		/* @brief Handles looking up the address for the vkCreateDebugUtilsMessengerEXT()
			  as it is an extension function. This function is our own proxy function
			  that handles this in the background.
		   @return Returns a VkResult(enum) on if the function was created successfully.
		*/
		VkResult create_debug_utils_messenger(const VkDebugUtilsMessengerCreateInfoEXT* a_create_info);

		/* @brief Proxy function that handles the need to clean up the VkDebugUtilsMessengerEXT
				  object with a destroy call.
		*/
		void destroy_debug_utils_messenger();

		static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
			VkDebugUtilsMessageSeverityFlagBitsEXT		a_message_severity,
			VkDebugUtilsMessageTypeFlagsEXT				a_message_type,
			const VkDebugUtilsMessengerCallbackDataEXT* a_callback_data,
			void* a_user_data);
	};
}

#endif // !VULKAN_VALIDATIONS_H