#include "ACFTEngine.h"

struct Pos
{
	float x;
	float y;
};

class MyLayer : public ACFT::Layer
{
public:
	MyLayer(ACFT::Ref<ACFT::Window> window)
		: m_Shader(ACFT::Shader::Create("resources/shaders/basic.shader"))
		, m_Camera(MakeRef<ACFT::OrthographicCamera>(window))
		, r_Renderer(ACFT::Renderer::GetRenderer())
		, r_Window(r_Renderer->GetWindow())
	{
		LoadPos();
		
		ACFT::Image left_0{ "resources/imgs/enemy_left_0.png", true };
		ACFT::Image left_1{ "resources/imgs/enemy_left_1.png", true };
		ACFT::Image left_2{ "resources/imgs/enemy_left_2.png", true };
		ACFT::Image left_3{ "resources/imgs/enemy_left_3.png", true };
		ACFT::Image left_4{ "resources/imgs/enemy_left_4.png", true };
		ACFT::Image left_5{ "resources/imgs/enemy_left_5.png", true };

		ACFT::Ref<ACFT::Atlas> atlas = ACFT::MakeRef<ACFT::Atlas>(6, 1);
		atlas->AddTexture(left_0);
		atlas->AddTexture(left_1);
		atlas->AddTexture(left_2);
		atlas->AddTexture(left_3);
		atlas->AddTexture(left_4);
		atlas->AddTexture(left_5);

		m_Sprite.UseAtlas(atlas);
		m_Sprite.SetInterval(70);

		r_Renderer->SetClearColor(0.5f, 0.8f, 0.9f, 1.0f);
		r_Renderer->EnableVSync();
		r_Renderer->EnableBlend();
	}

	~MyLayer()
	{
		SavePos();
	}

	void LoadPos()
	{
		using namespace ACFT::Serialization;
		auto pos_data = ACFT::DataFormat::ObjDataFile::Open("data/pos_data.ado");
		Pos default_pos;
		std::tie(default_pos.x, default_pos.y) = m_Camera->WindowPosToCamPos(r_Window->GetWidth(), r_Window->GetHeight(),
			200.0f, 200.0f);
		m_SpritePos = Deserialize<Pos>(ACFT::Codecs::PLAIN_CODEC<Pos>, pos_data).Or(default_pos);
	}

	void SavePos()
	{
		using namespace ACFT::Serialization;
		auto pos_data = ACFT::DataFormat::ObjDataFile::Open("data/pos_data.ado");
		Serialize<Pos>(m_SpritePos, ACFT::Codecs::PLAIN_CODEC<Pos>, pos_data);
	}

	virtual bool OnEvent(ACFT::Ref<ACFT::Event> event) override
	{
		if (event->GetType() == ACFT::Events::MOUSE_BUTTON)
		{
			auto& button = *event->GetInfo<ACFT::MouseButtonInfo>();
			if (button.keycode == ACFT::GLKeyCode::MOUSE_LEFT && !button.pressed)
			{
				//This is to ensure accurate position of the sprite.
				auto [xpos, ypos] = ACFT::GetCursorPos(r_Window);
				auto [cam_x, cam_y] = m_Camera->WindowPosToCamPos(r_Window->GetWidth(), r_Window->GetHeight(),
					static_cast<float>(xpos), static_cast<float>(ypos));
				m_SpritePos = { cam_x, cam_y };
				ACFT_LOG_INFO("Sprite moved to pos [x = {0}, y = {1}]!", cam_x, cam_y);
			}
		}

		return true;
	}

	virtual void OnUpdate(float time_step) override
	{
		m_Sprite.AccumulateTime(time_step);
	}

	virtual void OnRender() override
	{
		r_Renderer->BeginScene({ m_Camera });

		ACFT::RenderContext ctx{};
		ctx.shader = m_Shader;

		r_Renderer->DrawSprite(m_Sprite, m_SpritePos.x, m_SpritePos.y, 150.0f, 150.0f, ctx);

		r_Renderer->EndScene();
	}

private:
	ACFT::LockfreeQueue<ACFT::Event> m_EventQueue;
	ACFT::Sprite m_Sprite;
	ACFT::Ref<ACFT::Shader> m_Shader;
	ACFT::Ref<ACFT::OrthographicCamera> m_Camera;
	ACFT::Ref<ACFT::Renderer> r_Renderer;
	ACFT::Ref<ACFT::Window> r_Window;
	Pos m_SpritePos;
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

		auto timer = ACFT::NormalTimer();

		auto fps_profiler = ACFT::FPSProfiler();

		ACFT::Ref<ACFT::LayerStack> layers = ACFT::LayerStack::Create();

		ACFT::Ref<MyLayer> my_layer = ACFT::MakeRef<MyLayer>(window);
		layers->PushLayer(my_layer);

		while (!window->ShouldClose())
		{
			layers->OnUpdate(timer.GetElapsed());
			timer.Flush();

			layers->OnRender();

			window->PollEvents();

			//This is to display FPS.
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
