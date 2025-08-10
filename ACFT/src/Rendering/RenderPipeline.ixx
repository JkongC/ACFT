export module ACFT.RenderPipeline;

import <vector>;
import <variant>;

import Types;
import Shader;
import Tesselator;
import Camera;
import Texture;
import Sprite;
import UUID;
import ACFT.VectorRef;

namespace ACFT
{
	export struct Viewport
	{
		int x;
		int y;
		int width;
		int height;
	};

	export enum class RenderOp
	{
		none = 0, clear_all, clear_color, clear_depth
	};
	
	struct RenderPipeline;
	export struct RenderPass
	{
		struct SpriteDrawInfo
		{
			Ref<Sprite> sprite;
			int x;
			int y;
			int width;
			int height;
		};
		
		std::variant<Ref<Tesselator>, SpriteDrawInfo> object;
		Ref<Camera> camera = nullptr;
		Ref<Texture> texture = nullptr;
		UUID id{};
		Viewport viewport;
		RenderOp pre_op;
		RenderOp post_op;

		RenderPass() = default;

		void SetRenderObject(Ref<Tesselator> tesselator)
		{
			if (!tesselator)
				return;

			object = tesselator;
		}

		void SetRenderObject(Ref<Sprite> sprite, int x, int y, int width, int height)
		{
			if (!sprite)
				return;

			object = SpriteDrawInfo{ sprite, x, y, width, height };
		}

		bool operator==(const RenderPass& rhs) const
		{
			return id == rhs.id;
		}
	};

	export struct RenderPipeline
	{
		Ref<Shader> shader;
		std::vector<RenderPass> passes;
		Ref<RenderPipeline> next = nullptr;

		Ref<RenderPipeline> Append(Ref<RenderPipeline> n)
		{
			next = n;
			return n;
		}

		VectorRef<RenderPass, UUID> AppendPass(RenderPass pass)
		{
			passes.push_back(pass);
			return VectorRef<RenderPass, UUID>(
				&passes,
				passes.size() - 1,
				pass.id,
				[](const RenderPass& p, const UUID& id) -> bool
				{
					return p.id == id;
				}
			);
		}

		VectorRef<RenderPass, UUID> InsertPass(const RenderPass& prev, RenderPass pass)
		{
			for (auto it = passes.begin(); it != passes.end(); ++it)
			{
				if (prev == *it)
				{
					auto inserted_it = passes.insert(++it, pass);
					return VectorRef<RenderPass, UUID>(
						&passes,
						inserted_it - passes.begin(),
						pass.id,
						[](const RenderPass& p, const UUID& id) -> bool
						{
							return p.id == id;
						}
					);
				}
			}
		}

		void RemovePass(const RenderPass& pass)
		{
			for (auto it = passes.begin(); it != passes.end(); ++it)
			{
				if (pass == *it)
				{
					passes.erase(it);
					return;
				}
			}
		}

		void Clear()
		{
			passes.clear();
		}

		struct Iterator
		{
			RenderPipeline* pip;

			RenderPipeline& operator*() noexcept
			{
				return *pip;
			}

			Iterator& operator++() noexcept
			{
				pip = pip->next.get();
				return *this;
			}

			bool operator==(const Iterator& rhs) noexcept
			{
				return pip == rhs.pip;
			}

			bool operator!=(const Iterator& rhs) noexcept
			{
				return pip != rhs.pip;
			}
		};

		Iterator begin() noexcept
		{
			return Iterator{ this };
		}

		Iterator end() noexcept
		{
			return Iterator{ nullptr };
		}
	};
}