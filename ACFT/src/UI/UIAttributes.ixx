export module ACFT.UIAttributes;

import Types;
import Event;

namespace ACFT::UI
{
	export enum LocationType
	{
		distance, percentage
	};
	
	export struct ElementRectLocation
	{
		float top;
		float bottom;
		float left;
		float right;
		LocationType type;
	};

	export using UIAreaRect = ElementRectLocation;

	export template<typename T>
	class UIElement
	{
	public:
		void OnRender(this auto&& self)
		{
			self.OnRenderImpl();
		}

		void OnEvent(this auto&& self, const Ref<Event>& event)
		{
			self.OnEventImpl(event);
		}

		void OnUpdate(this auto&& self, float time_step)
		{
			self.OnUpdateImpl(time_step);
		}

	protected:
		void OnRenderImpl() {}
		void OnEventImpl(const Ref<Event>& event) {}
		void OnUpdateImpl(float time_step) {}

	protected:
		UIElement() = default;

		ElementRectLocation m_Location;
		bool m_ShouldRender;
	};
}
