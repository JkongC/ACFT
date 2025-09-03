export module ACFT.UIElementManager;

import <unordered_map>;

import Types;
import UUID;
import ACFT.UIAttributes;

namespace ACFT::UI
{
	class UIElementManager;
	
	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	struct UIElementRef
	{
		T* elem = nullptr;
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
		T& operator*() noexcept
		{
			return *elem;
		}

		bool CheckAvailablity(const UIElementManager& manager) const
		{
			return manager.IsElementAvailable(id);
		}

		void Release(UIElementManager& manager) noexcept
		{
			manager.RemoveElement(id);
			elem = nullptr;
		}
	};
	
	export class UIElementManager
	{
	public:
		/**
		 * 
		 * 
		 * \param ...args The args used to construct the element;
		 * \return The UIElementRef to the element;
		 */
		template<typename T, typename... Args>
			requires std::is_base_of_v<UIElement, T>
		UIElementRef<T> CreateElement(Args&&... args) noexcept
		{
			auto it = m_ElementPool.try_emplace(UUID{}, MakeScope(std::forward<Args>(args)...)).first;
			return { (*it).get(), UUID{} };
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

		template<typename T>
			requires std::is_base_of_v<UIElement, T>
		UIElementRef<T> FindElement(const UUID& id) const noexcept
		{
			auto it = m_ElementPool.find(id);
			if (it != m_ElementPool.end())
			{
				return { (*it).get(), id };
			}

			return {};
		}

		bool IsElementAvailable(const UUID& id) const noexcept
		{
			return m_ElementPool.find(id) != m_ElementPool.end();
		}

	private:
		std::unordered_map<UUID, Scope<UIElement>> m_ElementPool;
	};
}
