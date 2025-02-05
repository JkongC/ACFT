#ifndef ACFT_RESOURCEMANAGER_H_
#define ACFT_RESOURCEMANAGER_H_

#include "Render/Texture.h"

namespace ACFT
{
	class ResourceManager
	{
	public:
		static void Init();
		static void Shutdown();
		static ResourceManager& GetInstance();

		static void CreateTexture(const std::string& name, const std::string& filepath);
		static std::optional<Ref<Texture>> GetTexture(const std::string& name);
		static unsigned int GetTextureID(const std::string& name);
		static void RemoveTexture(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Texture>> textures;

	private:
		ResourceManager() = default;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
	};
}

#endif
