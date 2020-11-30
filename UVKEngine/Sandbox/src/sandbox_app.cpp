/* Uciniti engine includes
*/
#include <uciniti.h>

class vulkan_sandbox_layer : public Uciniti::layer
{
public:
	vulkan_sandbox_layer()
	{
	}

	virtual ~vulkan_sandbox_layer()
	{
	}

	/* on_attach() runs once at the start of the applications life. Occurs before the first rendered frame,
	*  and after application initialization. 
	*/
	virtual void on_attach() override
	{
		/* Create and load a shader, shader located within:
				"Sandbox/assets/shaders/<name>.glsl"
		*/
		Uciniti::renderer::get_shader_library()->load("assets/shaders/static_shader.glsl");

		/* Create the pipeline spec that will go with the shader.
		   NOTE: Pipeline creation to submission will be automated during the shaders reflection stage.
		*/
		Uciniti::pipeline_spec spec;
		spec._shader = Uciniti::renderer::get_shader_library()->get("static_shader");
		spec._shader->reload();
		spec._layout =
		{
			{ Uciniti::shader_data_type::_float3, "in_vert_position" },
			{ Uciniti::shader_data_type::_float3, "in_vert_normal" },
			{ Uciniti::shader_data_type::_float3, "in_vert_tangent" },
			{ Uciniti::shader_data_type::_float3, "in_vert_binormal" },
			{ Uciniti::shader_data_type::_float2, "in_vert_tex_coord" },
		};

		/* Create the pipeline using the spec.
		*/
		_mesh_pipeline = Uciniti::pipeline::create(spec);

		/* Submit the pipeline.
		*/
		Uciniti::renderer::submit_pipeline(_mesh_pipeline);

		/* Add a mesh to be rendered.
		*/
		_cerberus_mesh = Uciniti::mesh::create("assets/models/cerberus/Cerberus_LP.FBX");
	}

	virtual void on_detach() override
	{
	}

	virtual void on_update(Uciniti::time_step a_time_step) override
	{
		/* Example of key pressed.
		*/
		if (Uciniti::input::is_key_pressed(Uciniti::key_code::tab))
			UVK_TRACE("Tab key is pressed in update!");

		/* Translate, rotate, or scale the model.
		*/
		glm::mat4 cerberus_transform = glm::mat4(1.0f);
		cerberus_transform = glm::rotate(cerberus_transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		cerberus_transform = glm::scale(cerberus_transform, glm::vec3(0.01f, 0.01f, 0.01f));

		/* Set the final transform.
		*/
		_cerberus_mesh->set_transform(cerberus_transform);

		/* Submit the mesh to the renderer for the next render pass.
		*/
		Uciniti::renderer::submit_mesh(_cerberus_mesh);
	}

	virtual void on_event(Uciniti::event& a_event) override
	{
		/* Example of events dispatching.
		*/
		Uciniti::event_dispatcher dispatcher(a_event);

		/* Create an event dispatch for key presses.
		*/
		dispatcher.dispatch<Uciniti::key_pressed_event>(UVK_BIND_EVENT_FN(vulkan_sandbox_layer::on_key_pressed_event));

		/* Create an event dispatch for mouse presses.
		*/
		dispatcher.dispatch<Uciniti::mouse_button_pressed_event>(UVK_BIND_EVENT_FN(vulkan_sandbox_layer::on_mouse_pressed_event));
	}

	bool on_key_pressed_event(Uciniti::key_pressed_event& a_event)
	{
		switch (a_event.get_key_code())
		{
			case Uciniti::key_code::Q:
				UVK_TRACE("Q key is pressed in event!");
				break;
			case Uciniti::key_code::W:
				UVK_TRACE("W key is pressed in event!");
				break;
			case Uciniti::key_code::E:
				UVK_TRACE("E key is pressed in event!");
				break;
			case Uciniti::key_code::R:
				UVK_TRACE("R key is pressed in event!");
				break;
		}

		return false;
	}

	bool on_mouse_pressed_event(Uciniti::mouse_button_pressed_event& a_event)
	{
		UVK_TRACE("mouse button {0} pressed in event!", a_event.get_mouse_button());

		return false;
	}

private:
	Uciniti::ref_ptr<Uciniti::pipeline> _mesh_pipeline;
	Uciniti::ref_ptr<Uciniti::mesh> _cerberus_mesh;
};

/* @brief
*  @inherit
*/
class sandbox_app : public Uciniti::application
{
public:
	/***************************************************************/
	// Public Functions
	/***************************************************************/
	/* @brief
	*/
	sandbox_app()
	{
		push_layer(new vulkan_sandbox_layer());
	}

	/* @brief
	*/
	~sandbox_app()
	{
	}
};

/* Entry point declaration
*/
Uciniti::application* Uciniti::create_application()
{
	renderer_api::set_api(renderer_api_type::vulkan);
	return new sandbox_app();
}