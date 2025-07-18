module;

#include <entt/entity/registry.hpp>

#include "Platform.h"

export module Event;

export import EventInfo;

import <string>;
import <array>;
import <mutex>;

import Config.Thread;
import LockfreeQueue;
import Types;
import Log;
import Memory;
import ACFT.ObjectPool;

namespace ACFT
{
	class Event;
	class EventRegistry;
	class EventTypeBuilder;
	class EventRegistry;
	class EventManager;

	export class EventType
	{
	public:
		template<typename Info>
		bool HasInfo()
		{
			return EventRegistry::GetRegistry().try_get<Info>(m_Identifier) != nullptr;
		}

		ACFT_API bool IsSame(const Ref<EventType>& type);

		inline const std::string& GetName() const { return m_Name; }

		ACFT_API ~EventType();

		operator int() const
		{
			return static_cast<int>(m_Identifier);
		}

		friend bool operator==(const EventType& lhs, const EventType& rhs) noexcept
		{
			return lhs.m_Identifier == rhs.m_Identifier;
		}
	
	private:
		friend class EventRegistry;
		friend class EventTypeBuilder;
		ACFT_API EventType(entt::entity);

	private:
		entt::entity m_Identifier;
		std::string m_Name;
	};

	// A tool used in EventRegistry::Register to register an EventType.
	export class EventTypeBuilder
	{
	public:
		ACFT_API EventTypeBuilder();

		template<typename Info>
		EventTypeBuilder& Info()
		{
			if (m_BuildFunc)
			{
				m_BuildFunc = [prev_func = std::move(m_BuildFunc)](entt::entity type) -> void
					{
						prev_func(type);
						EventRegistry::GetRegistry().emplace<Info>(type);
					};
			}
			else
			{
				m_BuildFunc = [](entt::entity type) -> void
					{
						EventRegistry::GetRegistry().emplace<Info>(type);
					};
			}

			return *this;
		}

		/**
		 * Assign a name to the event type, so it can be displayed as a string.
		 * 
		 * \param name The name to be assign.
		 */
		ACFT_API EventTypeBuilder& Name(const std::string& name);

	private:
		friend class EventRegistry;
		std::function<void(entt::entity)> m_BuildFunc;

	private:
		std::string m_NameBuffer;
	};

	export class EventRegistry
	{
	public:
		static ACFT_API const Ref<EventType> Register(const EventTypeBuilder& type_builder);

		static ACFT_API bool AssertTypeExists(const Ref<EventType>& type);

		static ACFT_API const std::vector<Ref<EventType>>& GetAllTypes();
	
	private:
		friend class EventType;
		friend class EventTypeBuilder;
		EventRegistry() = default;
		static ACFT_API entt::registry& GetRegistry();

	private:
		static inline entt::registry s_Registry;
		static inline std::vector<Ref<EventType>> s_Types;
	};

	export class Event
	{
	public:
		ACFT_API Event(const Ref<EventType>& type);
		ACFT_API ~Event();

		template<typename T>
		void AttachInfo(T&& info)
		{
			if (!EventRegistry::AssertTypeExists(GetType()) || !AssertCanAttach<T>())
				return;

			EventManager::m_AllEvents.emplace<T>(m_Identifier, std::forward<T>(info));
		}

		template<typename T, typename... Args>
		void AttachInfo(Args&&... args)
		{
			if (!EventRegistry::AssertTypeExists(GetType()) || !AssertCanAttach<T>())
				return;

			EventManager::m_AllEvents.emplace<T>(m_Identifier, std::forward<Args>(args)...);
		}

		/**
		 * Get the info of an event.
		 *
		 * \return The pointer to the actual info. CAN BE NULLPTR.
		 */
		template<typename T>
		T* GetInfo()
		{
			return EventManager::m_AllEvents.try_get<T>(m_Identifier);
		}

		// Make the info of the event CONST.
		ACFT_API void Const();

		const Ref<EventType>& GetType() const { return m_Type; }

	private:
		template<typename T>
		bool AssertCanAttach()
		{
			if (!m_Mutable)
			{
				ACFT_LOG_WARN("[Event] Trying to modify a const event's info. Doing nothing instead.");
				return false;
			}

			if (!m_Type->HasInfo<T>())
			{
				ACFT_LOG_WARN("[Event] Trying to attach info to an incorrect type of event. Doing nothing instead.");
				return false;
			}

			return true;
		}

	private:
		entt::entity m_Identifier;
		Ref<EventType> m_Type;
		bool m_Mutable;
	};
	
	
	export enum EventManagerType
	{
		global = 0, layer, _Placeholder
	};
	
	export using SubscriberFunc = std::function<void(Ref<Event>)>;
	export using SubscriberMap = std::unordered_map<View<void>, SubscriberFunc>;
	export class EventManager
	{
	public:
		static ACFT_API EventManager& Global();
		
		void QueueEvent(Ref<Event> event);

		template<typename... Infos>
		void QueueEvent(const Ref<EventType>& type, Infos&&... infos)
		{
			Ref<Event> event = MakeRef<Event>(type);
			(event->AttachInfo<Infos>(std::forward<Infos>(infos)), ...);
			QueueEvent(event);
		}

		Ref<Event> PollEvent();

		void PollEventAndDistribute();

		ACFT_API void DistributeEvent(Ref<Event> event);

		template<typename... Infos>
		void DistributeEvent(const Ref<EventType>& type, Infos&&... infos)
		{
			Ref<Event> event = AllocateMakeRef<Event>(EventManager::m_EventPool, type);
			(event->AttachInfo<Infos>(std::forward<Infos>(infos)), ...);
			DistributeEvent(event);
		}

		template<typename Subscriber>
		void Subscribe(Ref<Subscriber> subscriber, const Ref<EventType>& type, SubscriberFunc callback)
		{
			if (!EventRegistry::AssertTypeExists(type))
				return;

			auto& map = m_Subscribers[type];
			map[subscriber] = std::move(callback);
		}

		template<typename Subscriber>
		void Unsubscribe(Ref<Subscriber> subscriber, const Ref<EventType>& type)
		{
			if (!EventRegistry::AssertTypeExists(type))
				return;

			auto& map = m_Subscribers[type];
			auto it = map.find(subscriber);
			if (it != map.end())
				map.erase(it);
		}

		ACFT_API EventManager() = default;
		ACFT_API EventManager& operator=(const EventManager&) = delete;
	
	private:
		friend class Event;

		static constexpr ObjectPoolMode s_PoolMode = Config::CompileTime::GetFunctionalThreadCount() == 0 ? ObjectPoolMode::single_threaded : ObjectPoolMode::multi_threaded;

		LockfreeQueue<Event, QueueNodeType::ref> m_EventQueue;
		std::unordered_map<int, SubscriberMap> m_Subscribers;
		static inline entt::registry m_AllEvents;
		static inline RefObjectPool<Event, s_PoolMode> m_EventPool{1000};
		std::mutex m_Mtx;
	};

	export namespace Events
	{
		// Below are some pre-defined event types, can be used directly.
		
		const Ref<EventType> MOUSE_POS = EventRegistry::Register(
			EventTypeBuilder().Info<MousePosInfo>()
		);

		const Ref<EventType> MOUSE_BUTTON = EventRegistry::Register(
			EventTypeBuilder().Info<MouseButtonInfo>()
		);

		const Ref<EventType> KEY = EventRegistry::Register(
			EventTypeBuilder().Info<KeyInfo>()
		);

		const Ref<EventType> WINDOW_RESIZE = EventRegistry::Register(
			EventTypeBuilder().Info<WindowSizeInfo>()
		);

		const Ref<EventType> SCROLL = EventRegistry::Register(
			EventTypeBuilder().Info<ScrollInfo>()
		);
	}
}