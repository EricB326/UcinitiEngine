#ifndef LOG_H
#define LOG_H

/* Vendor includes
*/
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

/* Uciniti engine includes
*/
#include "core.h"

namespace Uciniti
{
	class UCINITI_API log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& get_core_logger() { return core_logger; }
		inline static std::shared_ptr<spdlog::logger>& get_client_logger() { return client_logger; }

	private:
		static std::shared_ptr<spdlog::logger> core_logger;
		static std::shared_ptr<spdlog::logger> client_logger;

	};
}

/* @brief Core logging macros.
*/
#define UVK_CORE_TRACE(...) ::Uciniti::log::get_core_logger()->trace(__VA_ARGS__)
#define UVK_CORE_INFO(...)  ::Uciniti::log::get_core_logger()->info(__VA_ARGS__)
#define UVK_CORE_WARN(...)  ::Uciniti::log::get_core_logger()->warn(__VA_ARGS__)
#define UVK_CORE_ERROR(...) ::Uciniti::log::get_core_logger()->error(__VA_ARGS__)
#define UVK_CORE_FATAL(...) ::Uciniti::log::get_core_logger()->fatal(__VA_ARGS__)

/* @brief Client logging macros.
*/
#define UVK_TRACE(...)      ::Uciniti::log::get_client_logger()->trace(__VA_ARGS__)
#define UVK_INFO(...)       ::Uciniti::log::get_client_logger()->info(__VA_ARGS__)
#define UVK_WARN(...)       ::Uciniti::log::get_client_logger()->warn(__VA_ARGS__)
#define UVK_ERROR(...)      ::Uciniti::log::get_client_logger()->error(__VA_ARGS__)
#define UVK_FATAL(...)      ::Uciniti::log::get_client_logger()->fatal(__VA_ARGS__)

/* @brief If the build mode is distribution, strip the macros.
*/

#endif // !LOG_H