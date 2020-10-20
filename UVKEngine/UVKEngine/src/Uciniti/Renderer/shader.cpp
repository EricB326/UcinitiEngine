#include "uvkpch.h"
#include "shader.h"

#include <fstream>

namespace Uciniti
{
	std::string shader::create(const std::string& a_filepath)
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

	void shader::create_from_string(const std::string& a_source)
	{

	}
}