module;

#include <functional>
#include <atomic>

#include "Platform.h"

export module Application;

import Renderer;
import Window;
import Event;

namespace ACFT
{
	// Base class of user-defined application.
	export class Application
	{
	public:
		template<typename T, typename... Args>
			requires std::is_base_of_v<Application, T>
		static Ref<Application> Create(Args&&... args)
		{
			Ref<Application> app = MakeRef<T>(std::forward<Args>(args)...);
			auto& event_manager = EventManager::Global();
			auto event_callback = [=](Ref<Event> event) -> void { app->OnEvent(event); };
			for (auto& type : EventRegistry::GetAllTypes())
			{
				event_manager.Subscribe(app, type, event_callback);
			}
			
			return app;
		}

		ACFT_API static bool AppExisted() { return s_AppCreated; }

		ACFT_API virtual void Init() = 0;

		ACFT_API virtual void OnEvent(Ref<Event> event) = 0;
		ACFT_API virtual void OnUpdate(float time_step) = 0;
		ACFT_API virtual void OnRender() = 0;

		virtual ~Application() = default;

	private:
		static inline std::atomic<bool> s_AppCreated = false;
	};
}