#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H

#include "Uciniti/Renderer/shader.h"
#include "vulkan_base.h"
#include <glm/glm.hpp>
#include <spirv_glsl.hpp>
#include "Uciniti/Platform/Vulkan/vulkan_image.h"

namespace Uciniti
{
	class vulkan_shader : public shader
	{
	public:
		struct uniform_data
		{
			std::unordered_map<std::string, bool>		  _ubo_booleans;
			std::unordered_map<std::string, int>		  _ubo_ints;
			std::unordered_map<std::string, unsigned int> _ubo_uints;
			std::unordered_map<std::string, float>		  _ubo_floats;
			std::unordered_map<std::string, glm::vec2>	  _ubo_vec2;
			std::unordered_map<std::string, glm::vec3>	  _ubo_vec3;
			std::unordered_map<std::string, glm::vec4>	  _ubo_vec4;
			std::unordered_map<std::string, glm::mat2>	  _ubo_mat2;
			std::unordered_map<std::string, glm::mat3>	  _ubo_mat3;
			std::unordered_map<std::string, glm::mat4>	  _ubo_mat4;
		};

		struct uniform_buffer
		{
			VkBuffer _buffer;
			VkDescriptorBufferInfo _descriptor;
			VkDeviceMemory _memory;

			std::string _name;
			uint32_t _size = 0;
			uint32_t _binding_point = 0;

			uniform_data _member_data;
			VkShaderStageFlagBits _shader_stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct image_sampler_data
		{
			ref_ptr<vulkan_texture2D> _tex2d;
		};

		struct image_sampler
		{
			VkDescriptorImageInfo _descriptor;

			std::string _name;
			uint32_t _binding_point = 0;

			image_sampler_data _sampler_data;
			VkShaderStageFlagBits _shader_stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

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
		const VkDescriptorSetLayout& get_descriptor_set_layout() const { return _descriptor_set_layout; }

		VkDescriptorBufferInfo get_buffer_info(uint32_t a_binding = 0) { return _uniform_buffers[a_binding]._descriptor; }
		uniform_buffer get_uniform_buffer(uint32_t a_binding = 0) { return _uniform_buffers[a_binding]; }

		const std::vector<VkPipelineShaderStageCreateInfo> get_shader_stages() const { return shader_stages; }
		VkWriteDescriptorSet* get_write_descriptor_set(const std::string& a_key);
		VkDescriptorSet create_descriptor_set();

		bool get_member_bool(const uint32_t a_binding, const std::string& a_member_name);
		int	 get_member_int(const uint32_t a_binding, const std::string& a_member_name);
		unsigned int get_member_uint(const uint32_t a_binding, const std::string& a_member_name);
		float get_member_float(const uint32_t a_binding, const std::string& a_member_name);
		glm::vec2 get_member_vector2(const uint32_t a_binding, const std::string& a_member_name);
		glm::vec3 get_member_vector3(const uint32_t a_binding, const std::string& a_member_name);
		glm::vec4 get_member_vector4(const uint32_t a_binding, const std::string& a_member_name);
		glm::mat2 get_member_matrix2(const uint32_t a_binding, const std::string& a_member_name);
		glm::mat3 get_member_matrix3(const uint32_t a_binding, const std::string& a_member_name);
		glm::mat4 get_member_matrix4(const uint32_t a_binding, const std::string& a_member_name);
		void set_member_matrix2(const uint32_t a_binding, const std::string& a_member_name, const glm::mat2& a_data);
		void set_member_matrix3(const uint32_t a_binding, const std::string& a_member_name, const glm::mat3& a_data);
		void set_member_matrix4(const uint32_t a_binding, const std::string& a_member_name, const glm::mat4& a_data);

		void set_sampler2d(const uint32_t a_binding, ref_ptr<vulkan_texture2D> a_texture);

	private:
		/***************************************************************/
		// Private Variables
		/***************************************************************/
		std::string filepath;
		std::string _name;

		std::unordered_map<VkShaderStageFlagBits, std::string> shader_source;

		std::vector<VkPipelineShaderStageCreateInfo> shader_stages;

		std::unordered_map<uint32_t, uniform_buffer> _uniform_buffers;
		std::unordered_map<std::string, VkWriteDescriptorSet> _write_descriptor_sets;
		std::unordered_map<uint32_t, image_sampler> _image_samplers;

		VkDescriptorSetLayout _descriptor_set_layout;
		VkDescriptorSet _descriptor_set;
		VkDescriptorPool _descriptor_pool;
		
		VkShaderStageFlagBits shader_type_from_string(const std::string& a_type);

		std::unordered_map<VkShaderStageFlagBits, std::string> pre_process(const std::string& a_shader_source);
	
		void compile_to_spirv(std::array<std::vector<uint32_t>, 2>& a_binary_storage);
	
		void create_vertex_shader_module(VkPipelineShaderStageCreateInfo& a_shader_stage, const std::vector<uint32_t>& a_shader_data);
		void create_fragment_shader_module(VkPipelineShaderStageCreateInfo& a_shader_stage, const std::vector<uint32_t>& a_shader_data);
	
		void reflect(VkShaderStageFlagBits a_shader_stage, const std::vector<uint32_t>& a_shader_data);
		void reflected_member_to_cpp(const spirv_cross::SPIRType::BaseType& a_type, const uint32_t a_vec_size, const uint32_t a_columns, const std::string& a_name, const uint32_t a_binding);
		bool is_base_type_vec_mat(const uint32_t a_vec_size, const uint32_t a_columns, const std::string& a_name, const uint32_t a_binding);
		void create_descriptors();

		void allocate_uniform_buffer(uniform_buffer &a_dst);
		void allocate_image_sampler(image_sampler& a_image_dst);

		void retrieve_shader_name();

	};
}

#endif // !VULKAN_SHADER_H