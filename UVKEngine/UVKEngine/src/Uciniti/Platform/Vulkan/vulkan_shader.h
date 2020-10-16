#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H

#include "vulkan_base.h"

namespace Uciniti
{
	class vulkan_shader
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		vulkan_shader() {}

		/* @brief Constructs the variables to passed in data.
		*/
		vulkan_shader(const VkDevice& a_logical_device_handle, const char* a_file_path);

		/* @brief Destroys any memory created or created Vulkan functions.
		*/
		~vulkan_shader();

		/* @brief There is no copy constructor. If the compiler would otherwise
				  generate a copy call, it will instead fail.
		*/
		vulkan_shader(const vulkan_shader& a_other) = delete;

		/* @brief There is no assignment allowed.
		*/
		vulkan_shader& operator=(const vulkan_shader& a_other) = delete;

		/* @brief Using file i/o, read the SPIR-V shader files in binary.
				  Once all the data is read and stored, create the
				  shader module.
		*/
		void read_shader_from_file(const std::string& a_filepath);


		/* @brief Gets the shader code stored after loading a shader.
		   @return Vector of char with the shader code by reference.
		*/
		inline const std::string& get_shader_code() const { return shader_code; }

		/* @brief Gets the shader module handle.
		   @return VkShaderModule with the shader module handle by reference.
		*/
		const VkShaderModule& get_shader_module_handle() const;

	private:
		/***************************************************************/
		// Private Variables
		/***************************************************************/
		/* @brief The loaded code from the .spv file.
		*/
		std::string shader_code;


		/* @brief Creates a shader module and stores the handle. Created
				  using the stored shader code.
		*/
		void create_shader_module();
	};
}

#endif // !VULKAN_SHADER_H