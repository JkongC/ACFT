export module ACFT.Screen;

import <vector>;

import Types;
import Layer;
import Event;
import ACFT.UIAttributes;
import ACFT.Button;

namespace ACFT::UI
{
	class Menu;
	
	export class Screen : public Layer
	{
	public:
		virtual void OnEvent(Ref<Event> event) = 0;
		virtual void OnUpdate(float time_step) = 0;
		virtual void OnRender() = 0;

		virtual void OnOpen() = 0;
		virtual void OnClose() = 0;

		bool ShouldRender() const;
		void Show();
		void Hide();

		const ElementRectLocation& GetLocation() const;
		void SetLocation(const ElementRectLocation& location);

	protected:
		ElementRectLocation m_Location;
		Screen* m_Parent = nullptr;

		std::vector<Button> m_Buttons;
		std::vector<Menu> m_Menus;

		bool m_ShouldRender;
	};

	export class Menu : public Screen
	{

	};
}
