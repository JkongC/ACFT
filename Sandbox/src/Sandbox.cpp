#include "ACFTEngine.h"

#include <stdio.h>

class MyApp : public ACFT::Application
{
public:
	virtual int Entry(int argc, char** argv) override
	{
		ACFTItems items = ACFT::Initialize();
		auto& [window, renderer] = items;
		
		ACFT::Image left_0{"resources/imgs/enemy_left_0.png"};
		ACFT::Image left_1{"resources/imgs/enemy_left_1.png"};
		ACFT::Image left_2{"resources/imgs/enemy_left_2.png"};
		ACFT::Image left_3{"resources/imgs/enemy_left_3.png"};
		ACFT::Image left_4{"resources/imgs/enemy_left_4.png"};
		ACFT::Image left_5{"resources/imgs/enemy_left_5.png"};

		ACFT::Ref<ACFT::Atlas> atlas = ACFT::MakeRef<ACFT::Atlas>(6, 1);
		atlas->AddTexture(left_0);
		atlas->AddTexture(left_1);
		atlas->AddTexture(left_2);
		atlas->AddTexture(left_3);
		atlas->AddTexture(left_4);
		atlas->AddTexture(left_5);

		ACFT::Sprite sprite;
		sprite.SetInterval(500);
		sprite.UseAtlas(atlas);

		auto shader = ACFT::Shader::Create("resources/shaders/basic.shader");

		auto timer = ACFT::NormalTimer();
		while (!window->ShouldClose())
		{
			if (timer.GetElapsed() >= 1000)
			{
				printf("One second passed.\n");
				timer.Flush();
			}

			renderer.BeginScene({});

			ACFT::RenderContext ctx{};
			ctx.shader = shader;

			renderer.DrawSprite(sprite, 100, 100, 100, 100, ctx);

			renderer.EndScene();

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
