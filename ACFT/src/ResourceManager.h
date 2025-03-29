#ifndef ACFT_RESOURCEMANAGER_H_
#define ACFT_RESOURCEMANAGER_H_

#include "Render/Texture.h"
import UUID;

namespace ACFT
{
	class ResourceManager
	{
	public:
		static ResourceManager& GetInstance();

		static UUID CreateTexture(const std::string& name, const std::string& filepath);
		static std::optional<Ref<Texture>> GetTexture(const std::string& name);
		static Ref<Texture> GetTexture(const UUID& uuid);
		static void RemoveTexture(const std::string& name);
		static void RemoveTexture(const UUID& uuid);

	private:
		std::unordered_map<std::string, UUID> string_to_uuid;

		std::unordered_map<UUID, Ref<Texture>> textures;

	private:
		ResourceManager() = default;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
	};
}

#endif
