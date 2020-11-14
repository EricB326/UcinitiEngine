#include "uvkpch.h"
#include "shader.h"

#include <fstream>
#include "Uciniti/Renderer/renderer_api.h"
#include "Uciniti/Platform/Vulkan/vulkan_shader.h"

namespace Uciniti
{
	ref_ptr<shader> shader::create(const std::string& a_filepath)
	{
		switch (renderer_api::get_current_api())
		{
			case renderer_api_type::none: return nullptr;
			case renderer_api_type::vulkan: return create_ref_ptr<vulkan_shader>(a_filepath);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}

	std::string shader::read_from_file(const std::string& a_filepath)
	{
		// Shader code to return.
		std::string shader_code;

		// Open stream from given file as binary.
		std::ifstream file_input(a_filepath, std::ios::in | std::ios::binary);
		
		// Check if the file stream was not opened successfully. This results in an error.
		if (!file_input.is_open())
		{
			UVK_CORE_ERROR("Failed to load shader '{0}'", a_filepath);
			UVK_CORE_ASSERT(false, "");
		}
		else
		{
			// Move the file read position to the end of the file.
			file_input.seekg(0, std::ios::end);
			// Set the size of the vector to store the loaded file content.
			shader_code.resize(file_input.tellg());
			// Move the file read position back to the start of the file.
			file_input.seekg(0, std::ios::beg);
			// Read the file data into the buffer.
			file_input.read(&shader_code[0], shader_code.size());
		}
		
		// Close the file.
		file_input.close();
		// Return the shader code.
		return shader_code;
	}

	void shader::read_from_string(const std::string& a_source)
	{

	}

	void shader_library::add(const std::string& a_name, const ref_ptr<shader>& a_shader)
	{
		UVK_CORE_ASSERT(_shaders.find(a_name) == _shaders.end(), "Shader already exists!");
		_shaders[a_name] = a_shader;
	}

	void shader_library::add(const ref_ptr<shader>& a_shader)
	{
		std::string name = a_shader->get_name();
		add(name, a_shader);
	}

	void shader_library::load(const std::string& a_filepath)
	{
		auto shader_to_load = ref_ptr<shader>(shader::create(a_filepath));
		add(shader_to_load);
	}

	void shader_library::load(const std::string& a_name, const std::string& a_filepath)
	{
		auto shader = shader::create(a_filepath);
		add(a_name, shader);
	}

	const ref_ptr<shader>& shader_library::get(const std::string& a_name) const
	{
		UVK_CORE_ASSERT(_shaders.find(a_name) != _shaders.end(), "Shader not found!");
		return _shaders.at(a_name);
	}

}