#include "stdafx.h"

#include "ACFT.h"
#include "ResourceManager.h"

namespace ACFT
{
	static ResourceManager* rm_instance = nullptr;
	
	void ResourceManager::Init()
	{
		if (rm_instance == nullptr)
			rm_instance = new ResourceManager();
	}

	void ResourceManager::Shutdown()
	{
		if (rm_instance != nullptr)
		{
			delete rm_instance;
			rm_instance;
		}
	}

	ResourceManager& ResourceManager::GetInstance()
	{
		if (rm_instance == nullptr)
			Init();

		return *rm_instance;
	}

	void ResourceManager::CreateTexture(const std::string& name, const std::string& filepath)
	{
		auto& tex_map = GetInstance().textures;
		tex_map.try_emplace(name, MakeRef<Texture>(filepath));
	}

	std::optional<Ref<Texture>> ResourceManager::GetTexture(const std::string& name)
	{
		auto& tex_map = GetInstance().textures;
		if (tex_map.find(name) != tex_map.end())
		{
			return tex_map[name];
		}
		return std::nullopt;
	}

	unsigned int ResourceManager::GetTextureID(const std::string& name)
	{
		auto& tex_map = GetInstance().textures;
		if (tex_map.find(name) != tex_map.end())
		{
			return tex_map[name]->GetID();
		}
	}

	void ResourceManager::RemoveTexture(const std::string& name)
	{
		auto& tex_map = GetInstance().textures;
		auto it = tex_map.find(name);
		if (it != tex_map.end())
		{
			tex_map.erase(it);
		}
	}
}