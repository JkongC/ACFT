module;

#include "entt/entity/registry.hpp"

module Event;

import <mutex>;
import Log;

namespace ACFT
{
	EventType::EventType(entt::entity type)
		: m_Identifier(type), m_Name("")
	{ }

	EventType::~EventType()
	{
		EventRegistry::GetRegistry().destroy(m_Identifier);
	}

	template<typename Info>
	bool EventType::HasInfo()
	{
		return EventRegistry::GetRegistry().get<Info>(m_Identifier) != nullptr;
	}

	bool EventType::IsSame(Ref<EventType> type)
	{
		return m_Identifier == type->m_Identifier;
	}

	EventTypeBuilder::EventTypeBuilder()
		: m_BuildFunc(nullptr), m_NameBuffer("")
	{ }
	
	template<typename Info>
	EventTypeBuilder& EventTypeBuilder::Info()
	{
		if (m_BuildFunc)
		{
			m_BuildFunc = [prev_func = std::move(m_BuildFunc)](entt::entity type) -> void
				{
					prev_func(type);
					EventRegistry::m_Registry.emplace<Info>(type);
				};
		}
		else
		{
			m_BuildFunc = [](entt::entity type) -> void
				{
					EventRegistry::m_Registry.emplace<Info>(type);
				};
		}

		return *this;
	}

	EventTypeBuilder& EventTypeBuilder::Name(const std::string& name)
	{
		m_NameBuffer = name;
		return *this;
	}

	Event::Event(const Ref<EventType>& type)
		: m_Identifier(EventManager::m_AllEvents.create()), m_Type(type), m_Mutable(true)
	{ }

	Event::~Event()
	{
		EventManager::m_AllEvents.destroy(m_Identifier);
	}

	template<typename T>
	void Event::AttachInfo(T&& info)
	{
		if (!AssertTypeValid<T>() || !EventRegistry::AssertTypeExists(GetType()))
			return;

		EventManager::m_AllEvents.emplace_or_replace<T>(m_Identifier, std::forward<T>(info));
	}

	template<typename T, typename... Args>
	void Event::AttachInfo(Args&&... args)
	{
		if (!AssertTypeValid<T>() || !EventRegistry::AssertTypeExists(GetType()))
			return;

		EventManager::m_AllEvents.emplace_or_replace<T>(m_Identifier, std::forward<Args>(args)...);
	}

	template<typename T>
	T* Event::GetInfo()
	{
		return EventManager::m_AllEvents.get<T>(m_Identifier);
	}

	void Event::FreezeInfo()
	{
		m_Mutable = false;
	}

	template<typename T>
	bool Event::AssertCanAttach()
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

	const Ref<EventType> EventRegistry::Register(const EventTypeBuilder& type_builder)
	{
		entt::entity type = m_Registry.create();
		type_builder.m_BuildFunc(type);
		EventType* t = new EventType(type);
		t->m_Name = std::move(type_builder.m_NameBuffer);
		return Ref<EventType>(t);
	}

	entt::registry& EventRegistry::GetRegistry()
	{
		return m_Registry;
	}

	bool EventRegistry::AssertTypeExists(const Ref<EventType>& type)
	{
		if (!m_Registry.valid(type->m_Identifier))
		{
			ACFT_LOG_WARN("Event type \"{}\" doesn't exist or is not registered!", type->GetName());
			return false;
		}
		
		return true;
	}

	void EventManager::JoinEvent(Ref<Event> event)
	{
		ACFT_LOG_WARN("Event queue currently not supported!");
		return;
	}

	Ref<Event> EventManager::FetchEvent()
	{
		ACFT_LOG_WARN("Event queue currently not supported!");
		return nullptr;
	}

	void EventManager::DistributeEvent(Ref<Event> event)
	{
		if (!EventRegistry::AssertTypeExists(event->GetType()))
		{
			ACFT_LOG_WARN("Trying to distribute an event of an unregistered type!");
			return;
		}

		auto& map = m_Subscribers[event->GetType()];
		std::mutex mtx;
		std::lock_guard<std::mutex> lock(mtx);
		for (auto it = map.begin(); it != map.end();)
		{
			const auto& [obj_view, callback] = *it;
			if (auto sptr = obj_view.lock())
			{
				callback(event);
				++it;
			}
			else
			{
				it = map.erase(it);
			}
		}
	}

	template<typename... Infos>
	void EventManager::DistributeEvent(const Ref<EventType>& type, Infos&&... infos)
	{
		Ref<Event> event = MakeRef<Event>(type);
		(event->AttachInfo<Infos>(std::forward<Infos>(infos)), ...);
		DistributeEvent(event);
	}

	template<typename Subscriber>
	void EventManager::Subscribe(Ref<Subscriber> subscriber, const Ref<EventType>& type, SubscriberFunc callback)
	{
		if (!EventRegistry::AssertTypeExists(type))
			return;
		
		auto& map = m_Subscribers[type];
		map[subscriber] = std::move(callback);
	}

	template<typename Subscriber>
	void EventManager::Unsubscribe(Ref<Subscriber> subscriber, const Ref<EventType>& type)
	{
		if (!EventRegistry::AssertTypeExists(type))
			return;

		auto& map = m_Subscribers[type];
		auto it = map.find(subscriber);
		if (it != map.end())
			map.erase(it);
	}
}
