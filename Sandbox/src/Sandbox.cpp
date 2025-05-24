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
	MyLayer(Ref<Window> window)
		: m_Camera(MakeRef<OrthographicCamera>(window))
		, r_Renderer(Renderer::GetRenderer())
		, r_Window(r_Renderer->GetWindow())
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

	virtual void OnEvent(Ref<Event> event) override
	{
		const Ref<EventType>& type = event->GetType();
		if (type == ACFT::Events::MOUSE_BUTTON)
		{
			auto& button = *event->GetInfo<MouseButtonInfo>();
			if (button.keycode == ACFT::Keys::MOUSE_LEFT && button.pressed == false)
			{
				//This is to ensure accurate position of the sprite.
				auto [xpos, ypos] = GetCursorPos(r_Window);
				auto [cam_x, cam_y] = m_Camera->WindowPosToCamPos(r_Window->GetWidth(), r_Window->GetHeight(),
					static_cast<float>(xpos), static_cast<float>(ypos));
				m_SpritePos = { cam_x, cam_y };
				ACFT_LOG_INFO("Sprite moved to pos [x = {0}, y = {1}]!", cam_x, cam_y);
				m_MouseLeftPressed = false;
			}
		}
		else if (type == ACFT::Events::MOUSE_POS)
		{
			static const int DRAG_AREA = 80;
			
			auto& pos = *event->GetInfo<MousePosInfo>();
			if (pos.y <= DRAG_AREA)
			{
				r_Window->SetCursor(CursorType::pointing_hand);
				r_Window->SetDragAbility(true);
			}
			else
			{
				r_Window->SetCursor(CursorType::normal);
				r_Window->SetDragAbility(false);
			}
		}
		else if (type == ACFT::Events::KEY)
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
		m_Sprite.AccumulateTime(time_step);
		if (!m_Running)
			m_ShutdownProc(time_step);
	}

	Coroutine::TimestepTask<void> ShutdownProc()
	{
		float total_time = 0.0f;
		while (total_time <= 1100.0f)
		{
			total_time += co_await Coroutine::TimestepTask<void>::TimestepAwaitable{};
			r_Window->SetOpacity(1.0f - 0.7f * total_time / 1100.0f);
		}
		r_Window->MarkShouldClose();
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
	Ref<OrthographicCamera> m_Camera;
	Ref<Renderer> r_Renderer;
	Ref<Window> r_Window;
	Pos m_SpritePos;
	int m_WindowXCache;
	int m_WindowYCache;
	double m_CursorXCache;
	double m_CursorYCache;
	bool m_MouseLeftPressed = false;
	bool m_Running = true;
	Coroutine::TimestepTask<void> m_ShutdownProc = ShutdownProc();
};

class MyApp : public Application
{
public:
	virtual void Init() override
	{
		m_Layers = LayerStack::Create();
		m_Layers->PushLayer(MakeRef<MyLayer>(
			Renderer::GetRenderer()->GetWindow()
		));
	}

	virtual void OnUpdate(float time_step) override
	{
		m_Layers->OnUpdate(time_step);
	}

	virtual void OnRender() override
	{
		m_Layers->OnRender();
	}

private:
	Ref<LayerStack> m_Layers;
};

int main(int argc, char** argv)
{
	ACFT::Config::SetWindowIcon("resources/imgs/acft_icon.png");
	ACFT::Config::SetWindowName("Sandbox");
	ACFT::Config::UseFPSProfiler(false);

	ACFT::Engine::CreateApplication<MyApp>();
	return ACFT::Engine::Start(argc, argv);
}
