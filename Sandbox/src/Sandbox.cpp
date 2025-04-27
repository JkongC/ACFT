#include "ACFTEngine.h"

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
		sprite.SetInterval(150);
		sprite.UseAtlas(atlas);

		auto shader = ACFT::Shader::Create("resources/shaders/basic.shader");

		auto camera = MakeRef<ACFT::OrthographicCamera>(window);

		auto timer = ACFT::NormalTimer();
		renderer.SetClearColor(0.4f, 0.8f, 0.5f, 1.0f);
		renderer.EnableVSync();
		renderer.EnableBlend();

		double fps_log_timer = 0;
		size_t frame_count_rc = 0;

		while (!window->ShouldClose())
		{
			float elapsed = timer.GetElapsed();
			sprite.AccumulateTime(elapsed);
			fps_log_timer += elapsed;
			timer.Flush();

			if (fps_log_timer >= 1000.0)
			{
				ACFT_LOG_INFO("Current FPS: {}", static_cast<size_t>(frame_count_rc / (fps_log_timer / 1000.0)));
				fps_log_timer = 0;
				frame_count_rc = 0;
			}

			renderer.BeginScene({camera});

			ACFT::RenderContext ctx{};
			ctx.shader = shader;

			renderer.DrawSprite(sprite, 500.0f, 600.0f, 150.0f, 150.0f, ctx);

			renderer.EndScene();

			window->PollEvents();

			frame_count_rc++;
		}

		ACFT::Clean();

		return 0;
	}

	virtual void OnUpdate(time_t time_step) override {}
	virtual void OnRender() override {}
};

ACFT::Application* CreateApplication()
{
	return new MyApp();
}
