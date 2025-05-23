export module ACFT.Screen;

import Types;
import Layer;
import ACFT.Button;

namespace ACFT
{
	class Screen;
	export using Menu = Screen;
	
	export class Screen : public Layer
	{
	public:
		virtual void OnEvent(Ref<Event> event) override;

		virtual void OnUpdate(float time_step) override;

		virtual void OnRender() override;

		void OnOpen();

		void OnClose();

	private:
		Ref<Screen> m_Parent;

		std::vector<Button> m_Buttons;
		std::vector<Menu> m_Menus;
	};

	export class ScreenDefiner
	{
	
	};
}
