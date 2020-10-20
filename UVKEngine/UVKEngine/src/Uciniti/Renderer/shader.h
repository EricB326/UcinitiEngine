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

		std::string create(const std::string& a_filepath);
		void create_from_string(const std::string& a_source);


	private:

	};
}

#endif // !SHADER_H