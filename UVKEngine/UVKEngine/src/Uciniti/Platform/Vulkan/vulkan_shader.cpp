#include "uvkpch.h"
#include "vulkan_shader.h"

#include <fstream>

namespace Uciniti
{

	vulkan_shader::vulkan_shader(const VkDevice& a_logical_device_handle, const char* a_file_path)
	{

	}

	void vulkan_shader::read_shader_from_file(const std::string& a_filepath)
	{
		//// Open stream from given file as binary.
		//std::ifstream file_input(a_filepath, std::ios::binary | std::ios::ate); // Start at the end of the file with ios::ate.

		//// Check if the file stream was not opened successfully. This results in an error.
		//if (!file_input.is_open())
		//{
		//	UVK_CORE_ASSERT(false, "Could not load shader {0}", file_input);
		//}
		//else
		//{
		//	// Move the file read position to the end of the file.
		//	file_input.seekg(0, std::ios::end);
		//	// Set the size of the vector to store the loaded file content.
		//	shader_code.resize(file_input.tellg());
		//	// Move the file read position back to the start of the file.
		//	file_input.seekg(0, std::ios::beg);
		//	// Read the file data into the buffer.
		//	file_input.read(&shader_code[0], shader_code.size());
		//}

		//// Close the file.
		//file_input.close();
	}

	void vulkan_shader::create_shader_module()
	{

	}

	vulkan_shader::~vulkan_shader()
	{

	}

}