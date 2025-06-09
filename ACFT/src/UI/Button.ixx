export module ACFT.Button;

import <vector>;

import ACFT.UIAttributes;

namespace ACFT::UI
{
	export class Button : public UIElement<Button>
	{
	public:
		Button();

		void OnRenderImpl();

		void OnEventImpl(const Ref<Event>& event);

		void OnUpdateImpl(float time_step);

	private:
		
	};
}
