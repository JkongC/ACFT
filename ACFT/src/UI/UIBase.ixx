export module ACFT.UIBase;

import <unordered_map>;
import <vector>;

import Types;
import Event;
import Renderer;
import UUID;
import ACFT.Maths.Rectangle;

namespace ACFT::UI
{
	class UIElementManager;
	class UIElement;

	struct UIElementRef
	{
		UIElement* elem = nullptr;
		UUID id;

		operator bool() const noexcept
		{
			return elem != nullptr;
		}

		/**
		 * A risky shortcut to access the element.
		 *
		 * \return
		 */
		UIElement& operator*() noexcept
		{
			return *elem;
		}
	};

	export class UIElementManager
	{
	public:
		/**
		 * The ONLY correct way to create an UIElement.
		 *
		 * \param ...args The args used to construct the element;
		 * \return The UIElementRef to the element;
		 */
		template<typename T, typename... Args>
			requires std::is_base_of_v<UIElement, T>
		UIElementRef CreateElement(Args&&... args) noexcept
		{
			UUID id{};
			auto it = m_ElementPool.try_emplace(id, MakeScope<T>(std::forward<Args>(args)...)).first;
			return { (*it).second.get(), id };
		}

		/**
		 * Remove an element by its UUID. It is recommended that using UIElementRef's Release method, instead
		 * of directly calling this.
		 *
		 * \param id The UUID of the element
		 * \return
		 */
		void RemoveElement(const UUID& id) noexcept
		{
			m_ElementPool.erase(id);
		}

		UIElementRef FindElement(const UUID& id) const noexcept
		{
			auto it = m_ElementPool.find(id);
			if (it != m_ElementPool.end())
			{
				return { (*it).second.get(), id };
			}

			return {};
		}

		bool IsElementAvailable(const UIElementRef& ref) const noexcept
		{
			return IsElementAvailable(ref.id);
		}

		bool IsElementAvailable(const UUID& id) const noexcept
		{
			return m_ElementPool.find(id) != m_ElementPool.end();
		}

	private:
		std::unordered_map<UUID, Scope<UIElement>> m_ElementPool;
	};

	export struct ElementLocation
	{
		float x;
		float y;
	};

	export class UIElement
	{
	public:
		bool ShouldRender() const
		{
			return m_ShouldRender;
		}

		void Hide()
		{
			m_ShouldRender = false;
		}

		void Show()
		{
			m_ShouldRender = true;
		}

		const Maths::PositionRect& GetLocation() const
		{
			return m_Location;
		}

		void SetLocation(const Maths::PositionRect& location)
		{
			m_Location = location;
		}

		template<Maths::PositionType Dst>
		void CastLocationType(float area_width, float area_height)
		{
			m_Location.CastTo<Dst>(area_width, area_height);
		}

		void AddKidElement(Ref<UIElement> ele)
		{
			m_LogicKids.push_back(ele);
		}

		const Ref<UIElement>& GetParent() const
		{
			return m_LogicParent;
		}

		int GetDepth() const
		{
			return m_Depth;
		}

		void SetDepth(int depth)
		{
			m_Depth = depth;
		}

		virtual void OnEvent(Ref<Event> event) = 0;
		virtual void OnRender() = 0;
		virtual void OnUpdate(float time_step) = 0;

	protected:
		UIElement(Ref<UIElement> parent = nullptr, Maths::PositionRect location = {}, int depth = 0)
			: m_LogicParent(parent), m_Location(location), m_Depth(depth)
		{
		}

		std::vector<Ref<UIElement>> m_LogicKids;
		Ref<UIElement> m_LogicParent;
		Maths::PositionRect m_Location;
		int m_Depth;
		bool m_ShouldRender = true;
	};

	export template<typename T>
	struct DataBond
	{
		T* data;

		void BindTo(T* p_data) noexcept
		{
			data = p_data;
		}

		void UnBind() noexcept
		{
			data = nullptr;
		}

		void SetData(const T& new_data) noexcept
		{
			*data = new_data;
		}

		T& GetData() const noexcept
		{
			return *data;
		}

		void Increment() noexcept
		{
			if constexpr (requires(T obj) { ++obj; })
			{
				++(*data);
			}
		}

		void Decrement() noexcept
		{
			if constexpr (requires(T obj) { --obj; })
			{
				--(*data);
			}
		}

		T FetchAddAtomic(const T& arg, std::memory_order mem_order = std::memory_order_seq_cst) noexcept
		{
			if constexpr (requires(std::atomic_ref<T> obj) { ++obj; })
			{
				std::atomic_ref<T> atomic_data{ *data };
				return atomic_data.fetch_add(arg, mem_order);
			}
			else
			{
				return T{};
			}
		}

		T FetchSubAtomic(const T& arg, std::memory_order mem_order = std::memory_order_seq_cst) noexcept
		{
			if constexpr (requires(std::atomic_ref<T> obj) { --obj; })
			{
				std::atomic_ref<T> atomic_data{ *data };
				return atomic_data.fetch_sub(arg, mem_order);
			}
			else
			{
				return T{};
			}
		}

		operator bool() const noexcept
		{
			return data != nullptr;
		}
	};
}
