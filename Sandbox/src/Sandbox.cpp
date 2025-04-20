#include "ACFTEngine.h"

#include <stdio.h>

class MyApp : public ACFT::Application
{
public:
	virtual int Entry(int argc, char** argv) override
	{
		auto window = ACFT::Window::InitWindow();
		auto timer = ACFT::NormalTimer();
		while (!window->ShouldClose())
		{
			if (timer.GetElapsed() >= 1000)
			{
				printf("One second passed.\n");
				timer.Flush();
			}

			window->PollEvents();
		}

		return 0;
	}

	virtual void OnUpdate(time_t time_step) override {}
	virtual void OnRender() override {}
};

ACFT::Application* CreateApplication()
{
	return new MyApp();
}
