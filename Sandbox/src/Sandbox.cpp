#if defined (ACFT_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "ACFTEngine.h"

struct Pos
{
	float x;
	float y;
};

using namespace ACFT;

class MyLayer : public Layer
{
public:
	MyLayer()
		: r_Renderer(Renderer::Get())
		, m_Camera(MakeRef<OrthographicCamera>(Renderer::GetWindow()))
	{
		LoadPos();
		
		Image left_0{ "resources/imgs/enemy_left_0.png", true };
		Image left_1{ "resources/imgs/enemy_left_1.png", true };
		Image left_2{ "resources/imgs/enemy_left_2.png", true };
		Image left_3{ "resources/imgs/enemy_left_3.png", true };
		Image left_4{ "resources/imgs/enemy_left_4.png", true };
		Image left_5{ "resources/imgs/enemy_left_5.png", true };

		Ref<Atlas> atlas = MakeRef<Atlas>(6, 1);
		atlas->AddTexture(left_0);
		atlas->AddTexture(left_1);
		atlas->AddTexture(left_2);
		atlas->AddTexture(left_3);
		atlas->AddTexture(left_4);
		atlas->AddTexture(left_5);

		m_Sprite.UseAtlas(atlas);
		m_Sprite.SetInterval(80.0f);

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
		auto& window = Renderer::GetWindow();
		auto pos_data = ACFT::DataFormat::ObjDataFile::Open("data/pos_data.ado");
		Pos default_pos{};
		std::tie(default_pos.x, default_pos.y) = m_Camera->WindowPosToCamPos(window->GetWidth(), window->GetHeight(),
			200.0f, 200.0f);
		m_SpritePos = Deserialize<Pos>(ACFT::Codecs::PLAIN_CODEC<Pos>, pos_data).Or(default_pos);
	}

	void SavePos()
	{
		using namespace ACFT::Serialization;
		auto pos_data = ACFT::DataFormat::ObjDataFile::Open("data/pos_data.ado");
		Serialize<Pos>(m_SpritePos, ACFT::Codecs::PLAIN_CODEC<Pos>, pos_data);
	}

	virtual void OnEvent(Ref<Event> event) override
	{
		auto& window = Renderer::GetWindow();
		const Ref<EventType>& type = event->GetType();
		if (type == ACFT::Events::MOUSE_BUTTON)
		{
			auto& button = *event->GetInfo<MouseButtonInfo>();
			if (button.keycode == ACFT::Keys::MOUSE_LEFT && button.pressed == false)
			{
				//This is to ensure accurate position of the sprite.
				auto [xpos, ypos] = GetCursorPos(window);
				auto [cam_x, cam_y] = m_Camera->WindowPosToCamPos(window->GetWidth(), window->GetHeight(),
					static_cast<float>(xpos), static_cast<float>(ypos));
				m_SpritePos = { cam_x, cam_y };
				ACFT_LOG_INFO("Sprite moved to pos [x = {0}, y = {1}]!", cam_x, cam_y);
			}
		}
		else if (type == ACFT::Events::MOUSE_POS)
		{
			static const int DRAG_AREA = 80;
			
			auto& pos = *event->GetInfo<MousePosInfo>();
			if (pos.y <= DRAG_AREA)
			{
				window->SetCursor(CursorType::pointing_hand);
				window->SetDragAbility(true);
			}
			else
			{
				window->SetCursor(CursorType::normal);
				window->SetDragAbility(false);
			}
		}
	}

	virtual void OnUpdate(float time_step) override
	{
		m_Sprite.AccumulateTime(time_step);
	}

	virtual void OnRender() override
	{
		r_Renderer->BeginScene({ m_Camera });

		r_Renderer->DrawSprite(m_Sprite, m_SpritePos.x, m_SpritePos.y, 150.0f, 150.0f, {});

		r_Renderer->EndScene();
	}

private:
	LockfreeQueue<Event> m_EventQueue;
	Sprite m_Sprite;
	Ref<Renderer> r_Renderer;
	Ref<OrthographicCamera> m_Camera;
	Pos m_SpritePos;
};

class MyApp : public Application
{
public:
	virtual void Init() override
	{
		r_Layers = Renderer::GetWindow()->GetLayerStack();
		r_Layers->PushLayer(MakeRef<MyLayer>());
	}

	virtual void OnEvent(Ref<Event> event) override
	{
		const Ref<EventType>& type = event->GetType();
		if (type == ACFT::Events::KEY)
		{
			auto& key = *event->GetInfo<KeyInfo>();
			if (key.keycode == ACFT::Keys::ESCAPE && !key.pressed)
			{
				m_Running = false;
			}
		}
	}

	virtual void OnUpdate(float time_step) override
	{
		r_Layers->OnUpdate(time_step);
		if (!m_Running)
			m_ShutdownProc(time_step);
	}

	virtual void OnRender() override
	{
		r_Layers->OnRender();
	}

	Coroutine::TimestepTask<void> ShutdownProc()
	{
		auto& window = Renderer::GetWindow();
		float total_time = 0.0f;
		while (total_time <= 400.0f)
		{
			total_time += co_await Coroutine::TimestepTask<void>::TimestepAwaitable{};
			window->SetOpacity(1.0f - 0.8f * total_time / 400.0f);
		}
		window->SetOpacity(0.0f);
		window->MarkShouldClose();
	}

private:
	Ref<LayerStack> r_Layers;
	bool m_Running = true;
	Coroutine::TimestepTask<void> m_ShutdownProc = ShutdownProc();
};

#if defined (ACFT_PLATFORM_WINDOWS) && not defined (ACFT_ENABLE_LOG)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	{
		using namespace ACFT::Config;
		SetWindowIcon("resources/imgs/acft_icon.png");
		SetWindowName("Sandbox");
		UseFPSProfiler(false);
		SetWindowSize(1660, 1000);
	}

	ACFT::Engine::BindApplication(Application::Create<MyApp>());
	return ACFT::Engine::Start();
}
#else
int main(int argc, char** argv)
{
	{
		using namespace ACFT::Config;
		SetWindowIcon("resources/imgs/acft_icon.png");
		SetWindowName("Sandbox");
		UseFPSProfiler(false);
		SetWindowSize(1660, 1000);
	}

	ACFT::Engine::BindApplication(Application::Create<MyApp>());
	return ACFT::Engine::Start(argc, argv);
}
#endif


