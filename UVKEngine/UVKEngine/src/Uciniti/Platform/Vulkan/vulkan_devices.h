#ifndef VULKAN_DEVICES_H
#define VULKAN_DEVICES_H

#include <optional>
#include <unordered_set>

#include "vulkan_base.h"

namespace Uciniti
{
	/* @brief All the different queue family locations (indices) which are used during
		  the application.
	*/
	struct queue_family_indices
	{
		std::optional<uint32_t> graphics_family;     // Location of the Graphics Queue Family.
		std::optional<uint32_t> present_family;		 // Location of the Present Queue Family.
		std::optional<uint32_t> compute_family;		 // Location of the Compute Queue Family.
		std::optional<uint32_t> transfer_family;     // Location of the Transfer Queue Family.

		/* @brief Checks to see if all queue families are valid (has a value).
		   @return True if they are valid.
		*/
		bool is_valid()
		{
			return graphics_family.has_value() && present_family.has_value() && compute_family.has_value() && transfer_family.has_value();
		}
	};

	class vulkan_physical_device
	{
	public:
		vulkan_physical_device(const VkSurfaceKHR& a_surface);
		~vulkan_physical_device();

		static ref_ptr<vulkan_physical_device> select(const VkSurfaceKHR& a_surface);

		bool is_extension_supported(const std::string& a_extension_name);

		const VkPhysicalDevice& get_physical_device() const { return physical_device; }
		const queue_family_indices& get_queue_family_indices() const { return queue_families; }
		const uint32_t get_memory_type_index(uint32_t a_type_bits, VkMemoryPropertyFlags a_properties) const;
		const VkFormat get_depth_format() const { return _depth_format; }

	private:
		VkSurfaceKHR surface;

		VkPhysicalDevice physical_device;
		VkPhysicalDeviceProperties physical_device_properties;
		VkPhysicalDeviceFeatures physical_device_features;
		VkPhysicalDeviceMemoryProperties physical_device_mem_properties;

		queue_family_indices queue_families;

		VkFormat _depth_format;

		std::unordered_set<std::string> supported_extensions;

		VkPhysicalDevice select_physical_device(const std::vector<VkPhysicalDevice>& a_physical_devices);
		queue_family_indices find_queue_families(const int& a_queue_flags);
		void find_supported_extensions();
		void find_depth_format();
	};

	class vulkan_logical_device
	{
	public:
		vulkan_logical_device(const ref_ptr<vulkan_physical_device> a_physical_device, const VkPhysicalDeviceFeatures& a_physical_device_features);

		const VkDevice& get_logical_device() const { return logical_device; }
		const ref_ptr<vulkan_physical_device> get_physical_device_ref() const { return physical_device; }

		const VkQueue& get_graphics_queue_handle() const { return graphics_queue; }
		const VkCommandPool& get_command_pool() const { return command_pool; }

		void shutdown();

	private:
		VkDevice logical_device;
		ref_ptr<vulkan_physical_device> physical_device;
		VkCommandPool command_pool;

		VkPhysicalDeviceFeatures enabled_features;
		bool enable_debug_markers;

		VkQueue graphics_queue;
		VkQueue present_queue;

		std::vector<VkDeviceQueueCreateInfo> create_queue_infos();
		void create_queue_handles();
		void create_command_pool();


	};
}

#endif // !VULKAN_DEVICES_H