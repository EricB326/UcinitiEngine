#include "uvkpch.h"
#include "vulkan_shader.h"

#include "vulkan_context.h"

#include <shaderc/shaderc.hpp>

namespace Uciniti
{

	vulkan_shader::vulkan_shader(const std::string& a_filepath)
		: filepath(a_filepath)
	{
		reload();
	}

	vulkan_shader::~vulkan_shader()
	{

	}

	void vulkan_shader::reload()
	{
		// Only vertex and fragment shaders are supported for the time being.
		std::string source = create(filepath);

		// Pre-process the shader source to determine what is the vertex and what is the fragment shader.
		shader_source = pre_process(source);

		// Two stages only during vertex/fragment support only.
		shader_stages.resize(2);

		// Compile the shader stages.
		std::array<std::vector<uint32_t>, 2> shader_data;
		compile_to_spirv(shader_data);

		// Shader modules creation.
		//create_vertex_shader_module(shader_stages[0], shader_data[0]);
		//create_fragment_shader_module(shader_stages[1], shader_data[1]);
	}

	VkShaderStageFlagBits vulkan_shader::shader_type_from_string(const std::string& a_type)
	{
		if (a_type == "vertex")						   return VK_SHADER_STAGE_VERTEX_BIT;
		if (a_type == "fragment" || a_type == "pixel") return VK_SHADER_STAGE_FRAGMENT_BIT;
		if (a_type == "compute") UVK_CORE_ASSERT(false, "Compute shader stages not yet supported.");

		return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	}

	std::unordered_map<VkShaderStageFlagBits, std::string> vulkan_shader::pre_process(const std::string& a_shader_source)
	{
		// Shader type and source code to return.
		std::unordered_map<VkShaderStageFlagBits, std::string> shader_sources;

		// Find the stage token to determine the shader type.
		const char* stage_token = "#stage";
		size_t stage_token_length = strlen(stage_token);
		size_t token_pos = a_shader_source.find(stage_token, 0);

		// Whilst we are not at the end of the source file, attempt to locate the shader stage.
		while (token_pos != std::string::npos)
		{
			// Search the source for the first character that matches.
			size_t eol = a_shader_source.find_first_of("\r\n", token_pos);
			UVK_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			// See what stage of the shader it is by going to the first point after the stage token.
			size_t begin = token_pos + stage_token_length + 1;
			std::string stage_type = a_shader_source.substr(begin, eol - begin);
			UVK_CORE_ASSERT(stage_type == "vertex" || stage_type == "fragment" || stage_type == "pixel" || stage_type == "compute", "Invalid shader stage specified!");

			// Find the beginning of the next line.
			size_t next_line_pos = a_shader_source.find_first_not_of("\r\n", eol);
			// Find the next shader stage.
			token_pos = a_shader_source.find(stage_token, next_line_pos);

			// Store the first found shader type
			auto shader_type = shader_type_from_string(stage_type);
			// Store the shader source for this stage.
			shader_sources[shader_type] = a_shader_source.substr(next_line_pos, token_pos - (next_line_pos == std::string::npos ? a_shader_source.size() - 1 : next_line_pos));
		}

		return shader_sources;
	}

	void vulkan_shader::compile_to_spirv(std::array<std::vector<uint32_t>, 2>& a_binary_storage)
	{
		// Vertex shader stage.
		// If there is no binary (spir-v) stored, compile it for the stage.
		if (a_binary_storage[0].size() == 0)
		{
			// Setup the spir-v compiler and options.
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			// Set the target for the compilation.
			options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

			// To be changed...
			const bool optimize = false;
			if (optimize)
				options.SetOptimizationLevel(shaderc_optimization_level_performance);

			// Shader stage to compile.
			auto& shader_stage_source = shader_source.at(VK_SHADER_STAGE_VERTEX_BIT);

			// Compile the shader stage and verify all went well.
			shaderc::SpvCompilationResult compiled_stage = compiler.CompileGlslToSpv(shader_stage_source, shaderc_vertex_shader, filepath.c_str(), options);
			if (compiled_stage.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				UVK_CORE_ERROR(compiled_stage.GetErrorMessage());
				UVK_CORE_ASSERT(false, "Failed to compile vertex shader stage!");
			}

			// Store the compiled spir-v binary.
			a_binary_storage[0] = std::vector<uint32_t>(compiled_stage.cbegin(), compiled_stage.cend());
		}

		// Fragment shader stage.
		// If there is no binary (spir-v) stored, compile it for the stage.
		if (a_binary_storage[1].size() == 0)
		{
			// Setup the spir-v compiler and options.
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			// Set the target for the compilation.
			options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

			// To be changed...
			const bool optimize = false;
			if (optimize)
				options.SetOptimizationLevel(shaderc_optimization_level_performance);

			// Shader stage to compile.
			auto& shader_stage_source = shader_source.at(VK_SHADER_STAGE_FRAGMENT_BIT);

			// Compile the shader stage and verify all went well.
			shaderc::SpvCompilationResult compiled_stage = compiler.CompileGlslToSpv(shader_stage_source, shaderc_vertex_shader, filepath.c_str(), options);
			if (compiled_stage.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				UVK_CORE_ERROR(compiled_stage.GetErrorMessage());
				UVK_CORE_ASSERT(false, "Failed to compile vertex shader stage!");
			}

			// Store the compiled spir-v binary.
			a_binary_storage[1] = std::vector<uint32_t>(compiled_stage.cbegin(), compiled_stage.cend());
		}
	}

	//void vulkan_shader::create_vertex_shader_module(VkPipelineShaderStageCreateInfo& a_shader_stage, const std::vector<uint32_t>& a_shader_data)
	//{
	//	VkDevice device = vulkan_context::get_logical_device().get()->get_logical_device();
	//	UVK_CORE_ASSERT(a_shader_data.size(), "Invalid shader data!");
	//
	//	// Create a new shader module.
	//	VkShaderModuleCreateInfo module_create_info(vk_base_shader_module_create_info);
	//	module_create_info.codeSize = a_shader_data.size() * sizeof(uint32_t);
	//	module_create_info.pCode = a_shader_data.data();
	//
	//	VkShaderModule shader_module;
	//	VK_CHECK_RESULT(vkCreateShaderModule(device, &module_create_info, nullptr, &shader_module));
	//
	//	a_shader_stage = vk_base_pipeline_shader_stage_create_info;
	//	a_shader_stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
	//	a_shader_stage.module = shader_module;
	//	a_shader_stage.pName = "main";
	//}
	//
	//void vulkan_shader::create_fragment_shader_module(VkPipelineShaderStageCreateInfo& a_shader_stage, const std::vector<uint32_t>& a_shader_data)
	//{
	//	VkDevice device = vulkan_context::get_logical_device().get()->get_logical_device();
	//	UVK_CORE_ASSERT(a_shader_data.size(), "Invalid shader data!");
	//
	//	// Create a new shader module.
	//	VkShaderModuleCreateInfo module_create_info(vk_base_shader_module_create_info);
	//	module_create_info.codeSize = a_shader_data.size() * sizeof(uint32_t);
	//	module_create_info.pCode = a_shader_data.data();
	//
	//	VkShaderModule shader_module;
	//	VK_CHECK_RESULT(vkCreateShaderModule(device, &module_create_info, nullptr, &shader_module));
	//
	//	a_shader_stage = vk_base_pipeline_shader_stage_create_info;
	//	a_shader_stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	//	a_shader_stage.module = shader_module;
	//	a_shader_stage.pName = "main";
	//}

}