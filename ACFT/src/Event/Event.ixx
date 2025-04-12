export module Event;

import Base.entt;

export import EventInfo;

import <string>;

import LockfreeQueue;
import Types;

namespace ACFT
{
	class EventRegistry;
	class EventTypeBuilder;
	class EventRegistry;

	export class EventType
	{
	public:
		template<typename Info>
		bool HasInfo();

		bool IsSame(Ref<EventType> type);

		inline const std::string& GetName() const { return m_Name; }

		~EventType();
	
	private:
		friend class EventRegistry;
		friend class EventTypeBuilder;
		EventType(entt::entity);

	private:
		entt::entity m_Identifier;
		std::string m_Name;
	};

	// A tool used in EventRegistry::Register to register an EventType.
	export class EventTypeBuilder
	{
	public:
		EventTypeBuilder();

		template<typename Info>
		EventTypeBuilder& Info();

		/**
		 * Assign a name to the event type, so it can be displayed as a string.
		 * 
		 * \param name The name to be assign.
		 */
		EventTypeBuilder& Name(const std::string& name);

	private:
		friend class EventRegistry;
		std::function<void(entt::entity)> m_BuildFunc;

	private:
		std::string m_NameBuffer;
	};

	export class Event
	{
	public:
		Event(const Ref<EventType>& type);
		~Event();

		template<typename T>
		void AttachInfo(T&& info);
		template<typename T, typename... Args>
		void AttachInfo(Args&&... args);

		/**
		 * Get the info of an event.
		 * 
		 * \return The pointer to the actual info. CAN BE NULLPTR.
		 */
		template<typename T>
		T* GetInfo();

		// Make the info of the event CONST.
		void FreezeInfo();

		const Ref<EventType>& GetType() const { return m_Type; }

	private:
		template<typename T>
		bool AssertCanAttach();

	private:
		entt::entity m_Identifier;
		Ref<EventType> m_Type;
		bool m_Mutable;
	};

	export class EventRegistry
	{
	public:
		static const Ref<EventType> Register(const EventTypeBuilder& type_builder);

		static bool AssertTypeExists(const Ref<EventType>& type);
	
	private:
		friend class EventType;
		EventRegistry() = default;
		static entt::registry& GetRegistry();

	private:
		static inline entt::registry m_Registry;
	};
	
	using SubscriberFunc = std::function<void(Ref<Event>)>;
	export class EventManager
	{
	public:
		static void JoinEvent(Ref<Event> event);
		static Ref<Event> FetchEvent();

		static void DistributeEvent(Ref<Event> event);
		template<typename... Infos>
		static void DistributeEvent(const Ref<EventType>& type, Infos&&... infos);
		
		template<typename Subscriber>
		static void Subscribe(Ref<Subscriber> subscriber, const Ref<EventType>& type, SubscriberFunc callback);
		template<typename Subscriber>
		static void Unsubscribe(Ref<Subscriber> subscriber, const Ref<EventType>& type);

	private:
		friend class Event;
		static inline LockfreeQueue<Event> m_EventQueue;
		static inline std::unordered_map<Ref<EventType>, std::unordered_map<View<void>, SubscriberFunc, ViewHash<void>, ViewEqual<void>>> m_Subscribers;
		static inline entt::registry m_AllEvents;
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