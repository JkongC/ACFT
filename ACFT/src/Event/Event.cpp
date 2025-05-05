module;

#include <entt/entity/registry.hpp>

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

	bool EventType::IsSame(const Ref<EventType>& type)
	{
		return m_Identifier == type->m_Identifier;
	}

	EventTypeBuilder::EventTypeBuilder()
		: m_BuildFunc(nullptr), m_NameBuffer("")
	{ }

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

	void Event::Const()
	{
		m_Mutable = false;
	}

	const Ref<EventType> EventRegistry::Register(const EventTypeBuilder& type_builder)
	{
		entt::entity type = s_Registry.create();
		type_builder.m_BuildFunc(type);
		EventType* t = new EventType(type);
		t->m_Name = std::move(type_builder.m_NameBuffer);
		return s_Types.emplace_back(t);
	}

	entt::registry& EventRegistry::GetRegistry()
	{
		return s_Registry;
	}

	bool EventRegistry::AssertTypeExists(const Ref<EventType>& type)
	{
		if (!s_Registry.valid(type->m_Identifier))
		{
			ACFT_LOG_WARN("Event type \"{}\" doesn't exist or is not registered!", type->GetName());
			return false;
		}
		
		return true;
	}

	const std::vector<Ref<EventType>>& EventRegistry::GetAllTypes()
	{
		return s_Types;
	}

	EventManager& EventManager::Global()
	{
		static EventManager global_manager;
		return global_manager;
	}

	void EventManager::JoinEvent(Ref<Event> event)
	{
		ACFT_LOG_WARN("Event queue currently not supported!");
		return;
	}

	Ref<Event> EventManager::FetchEvent(bool consume)
	{
		ACFT_LOG_WARN("Event queue currently not supported!");
		return nullptr;
	}

	void EventManager::DistributeEvent(Ref<Event> event)
	{
		event->Const();
		
		if (!EventRegistry::AssertTypeExists(event->GetType()))
		{
			ACFT_LOG_WARN("Trying to distribute an event of an unregistered type!");
			return;
		}

		auto& map = m_Subscribers[event->GetType()];
		std::lock_guard<std::mutex> lock(m_Mtx);
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
}
