export module ACFT.Button;

import <vector>;

import Event;
import Types;
import ACFT.UIBase;

namespace ACFT::UI
{
	export class Button : public UIElement
	{
	public:
		Button(Ref<UIElement> parent = nullptr, Maths::PositionRect location = {}, int depth = 0);

		virtual void OnRender() {};
		virtual void OnEvent(Ref<Event> event) {};
		virtual void OnUpdate(float time_step) {};

	private:
		
	};
}
