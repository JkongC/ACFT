module;

#include <functional>
#include <atomic>

#include "Platform.h"

export module Application;

import Renderer;
import Window;

namespace ACFT
{
	// Base class of user-defined application.
	export class Application
	{
	public:
		ACFT_API static bool AppExisted() { return s_AppCreated; }

		ACFT_API virtual void Init() = 0;

		ACFT_API virtual void OnUpdate(float time_step) = 0;
		ACFT_API virtual void OnRender() = 0;

		virtual ~Application() = default;

	private:
		static inline std::atomic<bool> s_AppCreated = false;
	};
}