#ifndef RENDERER_H
#define RENDERER_H

#include "renderer_context.h"
#include "render_command.h"
#include "Uciniti/Renderer/render_pass.h"

#include "Uciniti/Core/application.h"
#include "Uciniti/Renderer/mesh.h"

namespace Uciniti
{
	class shader_library;

	class renderer
	{
	public:
		static void init();
		static void shutdown();

		static ref_ptr<renderer_context> get_context() 
		{
			return application::get().get_window().get_render_context();
		}

		template<typename FuncT>
		static void submit(FuncT&& func)
		{
			auto render_cmd = [](void* ptr) {
				auto p_func = (FuncT*)ptr;
				(*p_func)();

				// NOTE: Instead of destroying we could try and enforce all items to be trivially destructible
				// however some items like uniforms which contain std::strings still exist for now
				// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
				p_func->~FuncT();
			};
			auto storage_buffer = get_render_command_queue().allocate(render_cmd, sizeof(func));
			new (storage_buffer) FuncT(std::forward<FuncT>(func));
		}

		static void submit_pipeline(const ref_ptr<pipeline>& a_pipeline);

		static void submit_mesh(const ref_ptr<mesh>& a_mesh);

		static ref_ptr<shader_library> get_shader_library();

		static void wait_and_render();

	private:
		static render_command& get_render_command_queue();
	};
}

#endif // !RENDERER_H