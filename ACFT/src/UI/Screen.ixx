export module ACFT.Screen;

import <vector>;

import Types;
import Layer;
import Event;
import ACFT.UIAttributes;
import ACFT.Button;
import ACFT.ScreenQuadTree;

namespace ACFT::UI
{
	export class Menu : public UIElement
	{
	public:
		virtual void OnEvent(Ref<Event> event) = 0;
		virtual void OnUpdate(float time_step) = 0;
		virtual void OnRender() = 0;

		virtual void OnOpen() {}
		virtual void OnClose() {}

	private:

	};
}
