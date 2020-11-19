#include "uvkpch.h"
#include "vulkan_shader.h"

#include "vulkan_context.h"
#include "Uciniti/Platform/Vulkan/vulkan_buffer.h"

#include <shaderc/shaderc.hpp>

namespace Uciniti
{
	void vulkan_shader::reflected_member_to_cpp(const spirv_cross::SPIRType::BaseType& a_type, const uint32_t a_vec_size, const uint32_t a_columns, const std::string& a_name, const uint32_t a_binding)
	{
		switch (a_type)
		{
			case spirv_cross::SPIRType::BaseType::Boolean:
				UVK_CORE_TRACE("\t\tMember Type: Boolean");
				_uniform_buffers[a_binding]._member_data._ubo_booleans[a_name];
				return;
			case spirv_cross::SPIRType::BaseType::Int:
				UVK_CORE_TRACE("\t\tMember Type: Int");
				_uniform_buffers[a_binding]._member_data._ubo_ints[a_name];
				return;
			case spirv_cross::SPIRType::BaseType::UInt:
				UVK_CORE_TRACE("\t\tMember Type: UInt");
				_uniform_buffers[a_binding]._member_data._ubo_uints[a_name];
				return;
			case spirv_cross::SPIRType::BaseType::Float:
				if (is_base_type_vec_mat(a_vec_size, a_columns, a_name, a_binding)) return;
				UVK_CORE_TRACE("\t\tMember Type: Float");
				_uniform_buffers[a_binding]._member_data._ubo_floats[a_name];
				return;
			default:
				UVK_CORE_ERROR("Unknown spir-v base type!");
				return;
		}
	}

	bool vulkan_shader::is_base_type_vec_mat(const uint32_t a_vec_size, const uint32_t a_columns, const std::string& a_name, const uint32_t a_binding)
	{
		if (a_vec_size <= 1)
			return false;

		switch (a_columns)
		{
		case 2:
			UVK_CORE_TRACE("\t\tMember Type: Mat2");
			_uniform_buffers[a_binding]._member_data._ubo_mat2[a_name];
			return true;
		case 3:
			UVK_CORE_TRACE("\t\tMember Type: Mat3");
			_uniform_buffers[a_binding]._member_data._ubo_mat3[a_name];
			return true;
		case 4:
			UVK_CORE_TRACE("\t\tMember Type: Mat4");
			_uniform_buffers[a_binding]._member_data._ubo_mat4[a_name];
			return true;
		}

		switch (a_vec_size)
		{
		case 2:
			UVK_CORE_TRACE("\t\tMember Type: Vec2");
			_uniform_buffers[a_binding]._member_data._ubo_vec2[a_name];
			return true;
		case 3:
			UVK_CORE_TRACE("\t\tMember Type: Vec3");
			_uniform_buffers[a_binding]._member_data._ubo_vec3[a_name];
			return true;
		case 4:
			UVK_CORE_TRACE("\t\tMember Type: Vec4");
			_uniform_buffers[a_binding]._member_data._ubo_vec4[a_name];
			return true;
		}
	}

	vulkan_shader::vulkan_shader(const std::string& a_filepath)
		: filepath(a_filepath)
	{
		reload();
	}

	vulkan_shader::~vulkan_shader()
	{
		VkDevice device = vulkan_context::get()->get_logical_device()->get_logical_device();

		vkDestroyDescriptorPool(device, _descriptor_pool, nullptr);

		for (auto& this_ubo : _uniform_buffers)
		{
			vkDestroyBuffer(device, this_ubo.second._buffer, nullptr);
			vkFreeMemory(device, this_ubo.second._memory, nullptr);
		}

		vkDestroyDescriptorSetLayout(device, _descriptor_set_layout, nullptr);
	}

	void vulkan_shader::reload()
	{
		// Only vertex and fragment shaders are supported for the time being.
		std::string source = read_from_file(filepath);

		// Pre-process the shader source to determine what is the vertex and what is the fragment shader.
		shader_source = pre_process(source);

		// Two stages only during vertex/fragment support only.
		shader_stages.resize(2);

		// Compile the shader stages.
		std::array<std::vector<uint32_t>, 2> shader_data;
		compile_to_spirv(shader_data);

		// Shader modules creation.
		create_vertex_shader_module(shader_stages[0], shader_data[0]);
		create_fragment_shader_module(shader_stages[1], shader_data[1]);

		reflect(VK_SHADER_STAGE_VERTEX_BIT, shader_data[0]);
		reflect(VK_SHADER_STAGE_FRAGMENT_BIT, shader_data[1]);

		create_descriptors();

		// Grab the shaders name.
		retrieve_shader_name();
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
			// Search the source for the first instance of a new line or character return.
			size_t eol = a_shader_source.find_first_of("\r\n", token_pos);
			UVK_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			// See what stage of the shader it is by going to the first point after the stage token.
			size_t begin = token_pos + stage_token_length + 1;
			std::string stage_type = a_shader_source.substr(begin, eol - begin);
			UVK_CORE_ASSERT(shader_type_from_string(stage_type), "Invalid shader stage specified!");

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
		// #TODO: I should probably put this into a for each loop. Looping over each shader stage available.

		// Vertex shader stage.
		// If there is no binary (spir-v) stored, compile it for the stage.
		if (a_binary_storage[0].size() == 0)
		{
			// Setup the spir-v compiler and options.
			shaderc::Compiler compiler;
			shaderc::CompileOptions options;
			// Set the target for the compilation.
			options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

			// #TODO: To be changed...
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
			shaderc::SpvCompilationResult compiled_stage = compiler.CompileGlslToSpv(shader_stage_source, shaderc_fragment_shader, filepath.c_str(), options);
			if (compiled_stage.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				UVK_CORE_ERROR(compiled_stage.GetErrorMessage());
				UVK_CORE_ASSERT(false, "Failed to compile vertex shader stage!");
			}

			// Store the compiled spir-v binary.
			a_binary_storage[1] = std::vector<uint32_t>(compiled_stage.cbegin(), compiled_stage.cend());
		}
	}

	void vulkan_shader::create_vertex_shader_module(VkPipelineShaderStageCreateInfo& a_shader_stage, const std::vector<uint32_t>& a_shader_data)
	{
		VkDevice device = vulkan_context::get()->get_logical_device()->get_logical_device();
		UVK_CORE_ASSERT(a_shader_data.size(), "Invalid shader data!");
	
		// Create a new shader module.
		VkShaderModuleCreateInfo module_create_info(vk_base_shader_module_create_info);
		module_create_info.codeSize = a_shader_data.size() * sizeof(uint32_t);
		module_create_info.pCode = reinterpret_cast<const uint32_t*>(a_shader_data.data());
	
		VkShaderModule shader_module;
		VK_CHECK_RESULT(vkCreateShaderModule(device, &module_create_info, nullptr, &shader_module));
	
		a_shader_stage = vk_base_pipeline_shader_stage_create_info;
		a_shader_stage.stage = VK_SHADER_STAGE_VERTEX_BIT;
		a_shader_stage.module = shader_module;
		a_shader_stage.pName = "main";
	}
	
	void vulkan_shader::create_fragment_shader_module(VkPipelineShaderStageCreateInfo& a_shader_stage, const std::vector<uint32_t>& a_shader_data)
	{
		VkDevice device = vulkan_context::get()->get_logical_device()->get_logical_device();
		UVK_CORE_ASSERT(a_shader_data.size(), "Invalid shader data!");
	
		// Create a new shader module.
		VkShaderModuleCreateInfo module_create_info(vk_base_shader_module_create_info);
		module_create_info.codeSize = a_shader_data.size() * sizeof(uint32_t);
		module_create_info.pCode = reinterpret_cast<const uint32_t*>(a_shader_data.data());
	
		VkShaderModule shader_module;
		VK_CHECK_RESULT(vkCreateShaderModule(device, &module_create_info, nullptr, &shader_module));
	
		a_shader_stage = vk_base_pipeline_shader_stage_create_info;
		a_shader_stage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		a_shader_stage.module = shader_module;
		a_shader_stage.pName = "main";
	}

	void vulkan_shader::reflect(VkShaderStageFlagBits a_shader_stage, const std::vector<uint32_t>& a_shader_data)
	{
		VkDevice device = vulkan_context::get()->get_logical_device()->get_logical_device();

		UVK_CORE_TRACE("===========================");
		UVK_CORE_TRACE(" Vulkan Shader Reflection");
		UVK_CORE_TRACE(" {0}", filepath);
		UVK_CORE_TRACE("===========================");

		// Vertex Shader
		spirv_cross::Compiler compiler(a_shader_data);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		UVK_CORE_TRACE("Uniform Buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& name = resource.name;
			auto& buffer_type = compiler.get_type(resource.base_type_id);
			int member_count = buffer_type.member_types.size();
			uint32_t binding_point = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t size = compiler.get_declared_struct_size(buffer_type);

			UVK_CORE_ASSERT(_uniform_buffers.find(binding_point) == _uniform_buffers.end(), "");
			uniform_buffer& buffer = _uniform_buffers[binding_point];
			buffer._binding_point = binding_point;
			buffer._size = size;
			buffer._name = name;
			buffer._shader_stage = a_shader_stage;

			// _uniform_buffer[0]._data[in_vert_mvp]

			UVK_CORE_TRACE("\tName: {0}", name);
			UVK_CORE_TRACE("\tMember Count: {0}", member_count);
			for (size_t i = 0; i < member_count; i++)
			{
				std::string member_name = compiler.get_member_name(resource.base_type_id, i);
				auto& member_type = compiler.get_type(compiler.get_type(resource.type_id).member_types[i]);
				reflected_member_to_cpp(member_type.basetype, member_type.vecsize, member_type.columns, member_name, binding_point);
			}
			UVK_CORE_TRACE("\tBinding Point: {0}", binding_point);
			UVK_CORE_TRACE("\tSize: {0}", size);
			UVK_CORE_TRACE("-------------------");
		}
	}

	void vulkan_shader::create_descriptors()
	{
		VkDevice device = vulkan_context::get()->get_logical_device()->get_logical_device();

		std::vector<VkDescriptorPoolSize> type_counts;
		if (_uniform_buffers.size())
		{
			VkDescriptorPoolSize& type_count = type_counts.emplace_back();
			type_count.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			type_count.descriptorCount = _uniform_buffers.size();
		}

		VkDescriptorPoolCreateInfo descriptor_pool(vk_base_descriptor_pool_create_info);
		descriptor_pool.poolSizeCount = type_counts.size();
		descriptor_pool.pPoolSizes = type_counts.data();
		descriptor_pool.maxSets = 1;

		VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptor_pool, nullptr, &_descriptor_pool));

		std::vector<VkDescriptorSetLayoutBinding> layout_bindings;
		for (auto& [_binding, _uniform_buffer] : _uniform_buffers)
		{
			VkDescriptorSetLayoutBinding layout_binding;
			layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			layout_binding.descriptorCount = 1;
			layout_binding.stageFlags = _uniform_buffer._shader_stage;
			layout_binding.binding = _binding;
			layout_bindings.push_back(layout_binding);

			VkWriteDescriptorSet& set = _write_descriptor_sets[_uniform_buffer._name];
			set = {};
			set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			set.descriptorType = layout_binding.descriptorType;
			set.descriptorCount = 1;
			set.dstBinding = layout_binding.binding;

			allocate_uniform_buffer(_uniform_buffer);
		}

		VkDescriptorSetLayoutCreateInfo descriptor_layout(vk_base_descriptor_set_layout_create_info);
		descriptor_layout.bindingCount = layout_bindings.size();
		descriptor_layout.pBindings = layout_bindings.data();

		VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptor_layout, nullptr, &_descriptor_set_layout));

	}

	VkWriteDescriptorSet* vulkan_shader::get_write_descriptor_set(const std::string& a_key)
	{
		if (_write_descriptor_sets.find(a_key) == _write_descriptor_sets.end())
		{
			UVK_CORE_WARN("Shader {0} does not contain requested descriptor set {1}", _name, a_key);
			return nullptr;
		}
		return &_write_descriptor_sets.at(a_key);
	}

	VkDescriptorSet vulkan_shader::create_descriptor_set()
	{
		VkDevice device = vulkan_context::get()->get_logical_device()->get_logical_device();

		VkDescriptorSet descriptor_set;

		VkDescriptorSetAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool = _descriptor_pool;
		alloc_info.descriptorSetCount = 1;
		alloc_info.pSetLayouts = &_descriptor_set_layout;

		VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &alloc_info, &descriptor_set));
		_descriptor_set = descriptor_set;
		return descriptor_set;
	}

	glm::mat2 vulkan_shader::get_member_matrix2(const uint32_t a_binding, const std::string& a_member_name)
	{
		if (_uniform_buffers[a_binding]._member_data._ubo_mat2.find(a_member_name) != _uniform_buffers[a_binding]._member_data._ubo_mat2.end())
			return _uniform_buffers[a_binding]._member_data._ubo_mat2.at(a_member_name);
	}

	glm::mat3 vulkan_shader::get_member_matrix3(const uint32_t a_binding, const std::string& a_member_name)
	{
		if (_uniform_buffers[a_binding]._member_data._ubo_mat3.find(a_member_name) != _uniform_buffers[a_binding]._member_data._ubo_mat3.end())
			return _uniform_buffers[a_binding]._member_data._ubo_mat3.at(a_member_name);
	}

	glm::mat4 vulkan_shader::get_member_matrix4(const uint32_t a_binding, const std::string& a_member_name)
	{
		if (_uniform_buffers[a_binding]._member_data._ubo_mat4.find(a_member_name) != _uniform_buffers[a_binding]._member_data._ubo_mat4.end())
			return _uniform_buffers[a_binding]._member_data._ubo_mat4.at(a_member_name);
	}

	void vulkan_shader::set_member_matrix2(const uint32_t a_binding, const std::string& a_member_name, const glm::mat2& a_data)
	{
		if (_uniform_buffers[a_binding]._member_data._ubo_mat2.find(a_member_name) != _uniform_buffers[a_binding]._member_data._ubo_mat2.end())
			_uniform_buffers[a_binding]._member_data._ubo_mat2.at(a_member_name) = a_data;
	}

	void vulkan_shader::set_member_matrix3(const uint32_t a_binding, const std::string& a_member_name, const glm::mat3& a_data)
	{
		if (_uniform_buffers[a_binding]._member_data._ubo_mat3.find(a_member_name) != _uniform_buffers[a_binding]._member_data._ubo_mat3.end())
			_uniform_buffers[a_binding]._member_data._ubo_mat3.at(a_member_name) = a_data;
	}

	void vulkan_shader::set_member_matrix4(const uint32_t a_binding, const std::string& a_member_name, const glm::mat4& a_data)
	{
		if (_uniform_buffers[a_binding]._member_data._ubo_mat4.find(a_member_name) != _uniform_buffers[a_binding]._member_data._ubo_mat4.end())
			_uniform_buffers[a_binding]._member_data._ubo_mat4.at(a_member_name) = a_data;
	}

	void vulkan_shader::allocate_uniform_buffer(uniform_buffer& a_dst)
	{
		VkDevice device = vulkan_context::get()->get_logical_device()->get_logical_device();

		uniform_buffer& passed_uniform_buffer = a_dst;

		vulkan_buffer_util::create_buffer(vulkan_buffer_type::UNIFORM, passed_uniform_buffer._buffer, passed_uniform_buffer._memory, passed_uniform_buffer._size);
	
		passed_uniform_buffer._descriptor.buffer = passed_uniform_buffer._buffer;
		passed_uniform_buffer._descriptor.offset = 0;
		passed_uniform_buffer._descriptor.range = passed_uniform_buffer._size;
	}

	void vulkan_shader::retrieve_shader_name()
	{
		// Example of how this should work. If the shaders filepath is as follower:
		//		"assets/shaders/PBR.glsl"	   or		"assets\shaders\PBR.glsl"	
		// This code should find the final '/' or '\' character prior the actual file name and drop the filepath.
		// It should then find the last '.' after the '/' or '\' in order to drop the file extension.
		int final_slash = filepath.find_last_of("/\\");
		final_slash = (final_slash == std::string::npos) ? 0 : final_slash + 1;
		int final_period = filepath.rfind('.');
		int count = (final_period == std::string::npos) ? filepath.size() - final_slash : final_period - final_slash;

		_name = filepath.substr(final_slash, count);
	}
}