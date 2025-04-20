module;

#include <functional>
#include <atomic>

#include "Platform.h"

export module Application;

import Renderer;
import Window;

namespace ACFT
{
	export class Application
	{
	public:
		ACFT_API static bool AppExisted() { return s_AppCreated; }

		ACFT_API virtual int Entry(int argc, char** argv) = 0;

		ACFT_API virtual void OnUpdate(time_t time_step) = 0;
		ACFT_API virtual void OnRender() = 0;

	protected:
		Ref<Renderer> m_Renderer;
		Ref<Window> m_Window;

	private:
		static inline std::atomic<bool> s_AppCreated = false;
	};
}