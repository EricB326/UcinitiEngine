#ifndef SHADER_H
#define SHADER_H

#include <string.h>

namespace Uciniti
{
	class shader
	{
	public:
		virtual void reload() = 0;
		virtual void bind()   = 0;
		virtual void unbind() = 0;

		static ref_ptr<shader> create(const std::string& a_filepath);

		std::string read_from_file(const std::string& a_filepath);
		void read_from_string(const std::string& a_source);

		virtual const std::string& get_name() const = 0;
	};

	class shader_library
	{
	public:
		void add(const std::string& a_name, const ref_ptr<shader>& a_shader);
		void add(const ref_ptr<shader>& a_shader);

		/* @brief Allow the user to load the shader. By default the shader key will be the name of the file.
				  Example, assets/shaders/PBR.glsl will become PBR without the filepath or extension.
		*/
		void load(const std::string& a_filepath);

		/* @brief Same as the above load function but this time allow the user to name their shader key.
		*/
		void load(const std::string& a_name, const std::string& a_filepath);

		const ref_ptr<shader>& get(const std::string& a_name) const;

		void shutdown();

	private:
		std::unordered_map<std::string, ref_ptr<shader>> _shaders;

	};
}

#endif // !SHADER_H