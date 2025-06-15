export module ACFT.UIAttributes;

import Types;
import Event;
import Renderer;

namespace ACFT::UI
{
	export enum class LocationType
	{
		distance, percentage, pos
	};
	
	export struct ElementLocation
	{
		float x;
		float y;
	};
	
	export struct ElementLocationRect
	{
		float top;
		float bottom;
		float left;
		float right;
		LocationType type;
	};

	export using UIAreaRect = ElementLocationRect;

	export ElementLocationRect UIRectToType(const ElementLocationRect& rect, LocationType dst_type)
	{
		if (rect.type == dst_type)
		{
			return rect;
		}
		else
		{
			ElementLocationRect ret{};

			auto width = Renderer::GetWindow()->GetWidth();
			auto height = Renderer::GetWindow()->GetHeight();
			if (rect.type == LocationType::pos)
			{
				if (dst_type == LocationType::percentage)
				{

				}
				else
				{

				}
			}
			else if (rect.type == LocationType::percentage)
			{
				if (dst_type == LocationType::pos)
				{
					ret.top = height * rect.top;
					ret.bottom = height * (1.0f - rect.bottom);
					ret.left = width * rect.left;
					ret.right = width * (1.0f - rect.right);
				}
				else
				{

				}
			}
			else
			{
				if (dst_type == LocationType::pos)
				{
					ret.top = rect.top;
					ret.bottom = height - rect.bottom;
					ret.left = rect.left;
					ret.right = width - rect.right;
				}
				else
				{

				}
			}

			return ret;
		}
	}

	export bool UIRectCollided(ElementLocationRect one, ElementLocationRect other)
	{
		if (one.type != other.type)
			return false;

		if (one.type != LocationType::pos)
		{
			one = UIRectToType(one, LocationType::pos);
			other = UIRectToType(other, LocationType::pos);
		}
		
		bool vertical = (one.top >= other.top && one.top <= other.bottom) || (one.bottom >= other.top && one.bottom <= other.bottom);
		bool horizontal = (one.left >= other.left && one.left <= other.right) || (one.right >= other.left && one.right <= other.right);

		return vertical && horizontal;
	}

	export bool UIRectCollided(ElementLocationRect rect, ElementLocation point)
	{
		if (rect.type != LocationType::pos)
		{
			rect = UIRectToType(rect, LocationType::pos);
		}

		return point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom;
	}

	export class UIElement
	{
	public:
		bool ShouldRender() const
		{
			return m_ShouldRender;
		}

		void Hide()
		{
			m_ShouldRender = false;
		}

		void Show()
		{
			m_ShouldRender = true;
		}

		ElementLocation GetLocation() const
		{
			if (m_Location.type != LocationType::pos)
			{
				auto pos_rect = UIRectToType(m_Location, LocationType::pos);
				return { pos_rect.left, pos_rect.top };
			}

			return { m_Location.left, m_Location.top };
		}

		void SetLocation(ElementLocation location)
		{
			if (m_Location.type == LocationType::pos)
			{
				float x_offset = location.x - m_Location.left;
				float y_offset = location.y - m_Location.top;

				m_Location.left += x_offset;
				m_Location.right += x_offset;
				m_Location.top += y_offset;
				m_Location.bottom += y_offset;
			}
			else
			{
				auto pos_rect = UIRectToType(m_Location, LocationType::pos);

				float x_offset = location.x - pos_rect.left;
				float y_offset = location.y - pos_rect.top;

				pos_rect.left += x_offset;
				pos_rect.right += x_offset;
				pos_rect.top += y_offset;
				pos_rect.bottom += y_offset;

				m_Location = UIRectToType(pos_rect, m_Location.type);
			}
		}

		const ElementLocationRect& GetLocationRect() const
		{
			return m_Location;
		}

		void SetLocationRect(const ElementLocationRect& location)
		{
			m_Location = location;
		}

		void AddKidElement(Ref<UIElement> ele)
		{
			m_LogicKids.push_back(ele);
		}

		const Ref<UIElement>& GetParent() const
		{
			return m_LogicParent;
		}

		virtual void OnEvent(Ref<Event> event) = 0;
		virtual void OnRender() = 0;
		virtual void OnUpdate(float time_step) = 0;

	protected:
		std::vector<Ref<UIElement>> m_LogicKids;
		Ref<UIElement> m_LogicParent;
		ElementLocationRect m_Location;
		bool m_ShouldRender;
	};
}
