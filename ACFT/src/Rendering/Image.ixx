module;

#include "Platform.h"

export module Image;

export import <filesystem>;

namespace ACFT
{
	export class Image
	{
	public:
		ACFT_API Image();
		ACFT_API Image(const Image&);
		ACFT_API Image(Image&&) = default;
		ACFT_API Image(const std::filesystem::path& path, bool flip_vertically = false);
		ACFT_API ~Image();

		ACFT_API void InitImage(const std::filesystem::path& path, bool flip_vertically = false);

		ACFT_API inline unsigned char* GetInternalData() { return this->m_Data; }
		ACFT_API inline int GetWidth() const { return this->m_Width; }
		ACFT_API inline int GetHeight() const { return this->m_Height; }
		ACFT_API inline int GetChannels() const { return this->m_Channels; }
		ACFT_API inline std::filesystem::path GetImagePath() const { return this->m_ImagePath; }

	private:
		unsigned char* m_Data = nullptr;
		std::filesystem::path m_ImagePath;

		int m_Width;
		int m_Height;
		int m_Channels;
	};
}