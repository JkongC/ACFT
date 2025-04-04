export module Image;

import <filesystem>;

namespace ACFT
{
	export class Image
	{
	public:
		Image();
		Image(const Image&);
		Image(Image&&) = default;
		Image(std::filesystem::path path);
		~Image();

		void InitImage(std::filesystem::path path);

		inline unsigned char* GetInternalData() { return this->m_Data; }
		inline int GetWidth() const { return this->m_Width; }
		inline int GetHeight() const { return this->m_Height; }
		inline int GetChannels() const { return this->m_Channels; }
		inline std::filesystem::path GetImagePath() const { return this->m_ImagePath; }

	private:
		unsigned char* m_Data;
		std::filesystem::path m_ImagePath;

		int m_Width;
		int m_Height;
		int m_Channels;
	};
}