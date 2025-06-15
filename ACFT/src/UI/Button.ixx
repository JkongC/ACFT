export module ACFT.Button;

import <vector>;

import Event;
import Types;
import ACFT.UIAttributes;

namespace ACFT::UI
{
	export class Button : public UIElement
	{
	public:
		Button();

		virtual void OnRender();
		virtual void OnEvent(Ref<Event> event);
		virtual void OnUpdate(float time_step);

	private:
		
	};
}
