#ifndef ACFT_TEXTURE_H_
#define ACFT_TEXTURE_H_

namespace ACFT {
	class Texture
	{
	public:
		Texture() = delete;
		Texture(const std::string& file);
		~Texture();

		void Bind() const;

		inline unsigned int GetID() const { return texture_id; }

	private:
		unsigned int texture_id;
		unsigned char* local_buffer;
		std::string file_path;
		int width, height, BPP;
	};
}

#endif
