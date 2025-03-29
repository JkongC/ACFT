#include "stdafx.h"

#include "ACFT.h"
#include "Blocks.h"

namespace ACFT
{
	BlockContext&& BlockContext::Create()
	{
		BlockContext temp;
		return std::move(temp);
	}

	BlockContext&& BlockContext::Name(const std::string& block_name) &&
	{
		this->block_name = block_name;
		return std::move(*this);
	}

	BlockContext&& BlockContext::Texture(const std::string& texture_file) &&
	{
		this->texture = ResourceManager::CreateTexture(this->block_name, texture_file);
		return std::move(*this);
	}

	BlockContext::operator BlockContextID () const
	{
		return this->id;
	}

	BlockContext& BlockContext::operator=(BlockContext&& temp)
	{
		this->block_name = std::move(temp.block_name);
		this->texture = temp.texture;

		return *this;
	}

	BlockContext::BlockContext(BlockContext&& temp)
		: block_name(std::move(temp.block_name)), texture(std::move(temp.texture))
	{
	}

	const BlockContext& BlockRegistry::Register(BlockContext&& block)
	{
		auto[it, success] = registry.try_emplace(block_count++, std::move(block));
		return (*it).second;
	}
}