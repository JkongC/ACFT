export module ACFT.Screen;

import <vector>;

import Types;
import Layer;
import Event;
import ACFT.Button;

namespace ACFT
{
	export enum class ScreenLocationType
	{
		percent = 0, distance
	};

	export struct ScreenLocation
	{
		float left = 0.0f;
		float right = 0.0f;
		float top = 0.0f;
		float bottom = 0.0f;
		ScreenLocationType type;
	};
	
	export struct ScreenAttributes
	{
		ScreenLocation location;
		bool should_render;
	};
	
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

		const ScreenLocation& GetLocation() const;
		void SetLocation(ScreenLocation location);

	protected:
		ScreenAttributes m_Attr;

		Screen* m_Parent = nullptr;

		std::vector<Button> m_Buttons;
		std::vector<Menu> m_Menus;
	};

	export class Menu : public Screen
	{

	};
}
