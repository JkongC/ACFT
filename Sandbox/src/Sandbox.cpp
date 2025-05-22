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
		: m_Camera(ACFT::MakeRef<ACFT::OrthographicCamera>(window))
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

	virtual void OnEvent(ACFT::Ref<ACFT::Event> event) override
	{
		if (event->GetType() == ACFT::Events::MOUSE_BUTTON)
		{
			auto& button = *event->GetInfo<ACFT::MouseButtonInfo>();
			if (button.keycode == ACFT::Keys::MOUSE_LEFT && !button.pressed)
			{
				//This is to ensure accurate position of the sprite.
				auto [xpos, ypos] = ACFT::GetCursorPos(r_Window);
				auto [cam_x, cam_y] = m_Camera->WindowPosToCamPos(r_Window->GetWidth(), r_Window->GetHeight(),
					static_cast<float>(xpos), static_cast<float>(ypos));
				m_SpritePos = { cam_x, cam_y };
				ACFT_LOG_INFO("Sprite moved to pos [x = {0}, y = {1}]!", cam_x, cam_y);
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
	ACFT::LockfreeQueue<ACFT::Event> m_EventQueue;
	ACFT::Sprite m_Sprite;
	ACFT::Ref<ACFT::OrthographicCamera> m_Camera;
	ACFT::Ref<ACFT::Renderer> r_Renderer;
	ACFT::Ref<ACFT::Window> r_Window;
	Pos m_SpritePos;
};

class MyApp : public ACFT::Application
{
public:
	virtual void Init() override
	{
		m_Layers = ACFT::LayerStack::Create();
		m_Layers->PushLayer(ACFT::MakeRef<MyLayer>(
			ACFT::Renderer::GetRenderer()->GetWindow()
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
	ACFT::Ref<ACFT::LayerStack> m_Layers;
};

int main(int argc, char** argv)
{
	ACFT::Config::SetWindowIcon("resources/imgs/acft_icon.png");
	ACFT::Config::SetWindowName("Sandbox");
	ACFT::Config::UseFPSProfiler(false);

	ACFT::Engine::CreateApplication<MyApp>();
	return ACFT::Engine::Start(argc, argv);
}
