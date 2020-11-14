#ifndef TEXTURE_H
#define TEXTURE_H

namespace Uciniti
{
	class texture
	{
	public:
		virtual ~texture();

		virtual void bind(uint32_t a_slot = 0) const = 0;

		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;
		//virtual uint32_t get_mip_level_count() const = 0;

	private:

	};

	class texture2D : public texture
	{
	public:
		static ref_ptr<texture2D> create(const std::string& a_path);
	};
}

#endif // !TEXTURE_H