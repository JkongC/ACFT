#include "ACFTEngine.h"

class MyLayer : public ACFT::Layer
{
protected:
	MyLayer()
	{

	}

	virtual bool OnEvent(ACFT::Ref<ACFT::Event> event) override
	{

	}

	virtual void OnUpdate(float time_step) override
	{

	}

	virtual void OnRender() override
	{
		auto& renderer = ACFT::Renderer::GetRenderer();

	}

private:
	ACFT::LockfreeQueue<ACFT::Event> m_EventQueue;
	ACFT::Sprite m_Sprite;
};

class MyApp : public ACFT::Application
{
public:
	virtual int Entry(int argc, char** argv) override
	{
		ACFT::Config::SetWindowIcon("resources/imgs/acft_icon.png");
		ACFT::Config::SetWindowName("Sandbox");
		
		ACFTItems items = ACFT::Initialize();
		auto& [window, renderer] = items;
		
		ACFT::Image left_0{"resources/imgs/enemy_left_0.png", true};
		ACFT::Image left_1{"resources/imgs/enemy_left_1.png", true};
		ACFT::Image left_2{"resources/imgs/enemy_left_2.png", true};
		ACFT::Image left_3{"resources/imgs/enemy_left_3.png", true};
		ACFT::Image left_4{"resources/imgs/enemy_left_4.png", true};
		ACFT::Image left_5{"resources/imgs/enemy_left_5.png", true};

		ACFT::Ref<ACFT::Atlas> atlas = ACFT::MakeRef<ACFT::Atlas>(6, 1);
		atlas->AddTexture(left_0);
		atlas->AddTexture(left_1);
		atlas->AddTexture(left_2);
		atlas->AddTexture(left_3);
		atlas->AddTexture(left_4);
		atlas->AddTexture(left_5);

		ACFT::Sprite sprite;
		sprite.SetInterval(70);
		sprite.UseAtlas(atlas);

		auto shader = ACFT::Shader::Create("resources/shaders/basic.shader");

		auto camera = MakeRef<ACFT::OrthographicCamera>(window);

		auto timer = ACFT::NormalTimer();
		renderer->SetClearColor(0.4f, 0.8f, 0.5f, 1.0f);
		renderer->EnableVSync();
		renderer->EnableBlend();

		auto fps_profiler = ACFT::FPSProfiler();

		while (!window->ShouldClose())
		{
			float elapsed = timer.GetElapsed();
			sprite.AccumulateTime(elapsed);
			timer.Flush();

			renderer->BeginScene({camera});

			ACFT::RenderContext ctx{};
			ctx.shader = shader;

			renderer->DrawSprite(sprite, 500.0f, 600.0f, 150.0f, 150.0f, ctx);

			renderer->EndScene();

			window->PollEvents();

			fps_profiler.RecordFrame();
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
