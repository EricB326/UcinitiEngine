#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H

#include "Uciniti/Renderer/shader.h"
#include "vulkan_base.h"

namespace Uciniti
{
	class vulkan_shader : public shader
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Constructs the variables to passed in data.
		*/
		vulkan_shader(const std::string& a_filepath);

		/* @brief Destroys any memory created or created Vulkan functions.
		*/
		~vulkan_shader();

		virtual void bind() override {}
		virtual void unbind() override {}
		void reload() override;

		virtual const std::string& get_name() const override { return _name; }

		const std::vector<VkPipelineShaderStageCreateInfo> get_shader_stages() const { return shader_stages; }

	private:
		/***************************************************************/
		// Private Variables
		/***************************************************************/
		std::string filepath;
		std::string _name;

		std::unordered_map<VkShaderStageFlagBits, std::string> shader_source;

		std::vector<VkPipelineShaderStageCreateInfo> shader_stages;

		VkShaderStageFlagBits shader_type_from_string(const std::string& a_type);

		std::unordered_map<VkShaderStageFlagBits, std::string> pre_process(const std::string& a_shader_source);
	
		void compile_to_spirv(std::array<std::vector<uint32_t>, 2>& a_binary_storage);
	
		void create_vertex_shader_module(VkPipelineShaderStageCreateInfo& a_shader_stage, const std::vector<uint32_t>& a_shader_data);
		void create_fragment_shader_module(VkPipelineShaderStageCreateInfo& a_shader_stage, const std::vector<uint32_t>& a_shader_data);
	
		void retrieve_shader_name();
	};
}

#endif // !VULKAN_SHADER_H