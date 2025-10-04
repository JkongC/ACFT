export module ACFT.Maths.Rectangle;

import <concepts>;
import <array>;
import <string_view>;

#include "Reflection.h"

namespace ACFT::Maths
{
	export struct Rect
	{
		float top;
		float bottom;
		float left;
		float right;
	};

	export bool CheckRectCollision(const Rect& r1, const Rect& r2)
	{
		return !(r1.top > r2.bottom || r1.bottom < r2.top) && !(r1.right < r2.left || r1.left > r2.right);
	}

	CREATE_ENUM(PositionType, position, border_dist, screen_ratio)
	
	export template<PositionType From, PositionType To>
		requires (From != To)
	Rect PositionRectCast(const Rect& src, float area_width, float area_height);

	export template<>
	Rect PositionRectCast<PositionType::position, PositionType::border_dist>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top,
			area_height - src.bottom,
			src.left,
			area_width - src.right
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::border_dist, PositionType::position>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top,
			area_height - src.bottom,
			src.left,
			area_width - src.right
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::position, PositionType::screen_ratio>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top / area_height,
			src.bottom / area_height,
			src.left / area_width,
			src.right / area_width
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::screen_ratio, PositionType::position>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top * area_height,
			src.bottom * area_height,
			src.left * area_width,
			src.right * area_width
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::border_dist, PositionType::screen_ratio>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top / area_height,
			(area_height - src.bottom) / area_height,
			src.left / area_width,
			(area_width - src.right) / area_width
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::screen_ratio, PositionType::border_dist>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top * area_height,
			area_height - src.bottom * area_height,
			src.left * area_width,
			area_width - src.right * area_width
		};
	}

	export Rect PositionRectCast(const Rect& src, PositionType dst_type, float area_width, float area_height)
	{
		switch (dst_type)
		{
		case ACFT::Maths::PositionType::position:
			break;
		case ACFT::Maths::PositionType::border_dist:
			break;
		case ACFT::Maths::PositionType::screen_ratio:
			break;
		default:
			break;
		}
	}
	
	export struct PositionRect : public Rect
	{
		PositionRect() = default;
		PositionRect(const PositionRect&) = default;
		PositionRect(PositionType type) : Rect(), m_Type(type) {}
		PositionRect(const Rect& r) : Rect(r) {}
		PositionRect(const Rect& r, PositionType type) : Rect(r), m_Type(type) {}

		PositionType GetType() const { return m_Type; }

		bool IsSameType(const PositionRect& other) const
		{
			return m_Type == other.m_Type;
		}

		Rect& AsRect(this PositionRect& self)
		{
			return static_cast<Rect&>(self);
		}

		PositionRect& operator=(this PositionRect& self, const Rect& r)
		{
			auto& [top, bottom, left, right] = r;
			return self;
		}

		template<PositionType Dst>
		void CastTo(this PositionRect& self, float area_width, float area_height)
		{
			if (m_Type == Dst)
				return;
			
			switch (m_Type)
			{
			case PositionType::position:
				AsRect() = PositionRectCast<PositionType::position, Dst>(self, area_width, area_height);
				break;
			case PositionType::border_dist:
				AsRect() = PositionRectCast<PositionType::border_dist, Dst>(self, area_width, area_height);
				break;
			case PositionType::screen_ratio:
				AsRect() = PositionRectCast<PositionType::screen_ratio, Dst>(self, area_width, area_height);
				break;
			default:
				break;
			}

			m_Type = Dst;
		}
	private:
		PositionType m_Type = PositionType::position;
	};
}
