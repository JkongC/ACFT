#include "stdafx.h"

#include "ACFT.h"
#include "ResourceManager.h"

namespace ACFT
{
	ResourceManager& ResourceManager::GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	UUID ResourceManager::CreateTexture(const std::string& name, const std::string& filepath)
	{
		auto& instance = GetInstance();
		UUID uuid{};
		instance.string_to_uuid.insert({ name, uuid });
		instance.textures.try_emplace(uuid, MakeRef<Texture>(filepath));
		
		return uuid;
	}

	std::optional<Ref<Texture>> ResourceManager::GetTexture(const std::string& name)
	{
		auto& instance = GetInstance();
		if (instance.string_to_uuid.find(name) != instance.string_to_uuid.end())
		{
			return GetTexture(instance.string_to_uuid[name]);
		}
		return std::nullopt;
	}

	Ref<Texture> ResourceManager::GetTexture(const UUID& uuid)
	{
		auto& textures = GetInstance().textures;
		return textures[uuid];
	}

	void ResourceManager::RemoveTexture(const std::string& name)
	{
		auto& uuids = GetInstance().string_to_uuid;
		auto it = uuids.find(name);
		if (it != uuids.end())
		{
			RemoveTexture(uuids[name]);
		}
	}

	void ResourceManager::RemoveTexture(const UUID& uuid)
	{
		auto& textures = GetInstance().textures;
		auto it = textures.find(uuid);
		if (it != textures.end())
		{
			textures.erase(it);
		}
	}
}