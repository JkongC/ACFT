#ifndef ACFT_TEXTURE_H_
#define ACFT_TEXTURE_H_

class Texture
{
public:
	Texture() = delete;
	Texture(const std::string& file);
	~Texture() = default;

private:
	unsigned int texture_id;
	unsigned char local_buffer;
	std::string file_path;
	int width, height, BPP;
};

#endif
