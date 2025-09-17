export module ACFT.Maths.Rectangle;

import <concepts>;

namespace ACFT::Maths
{
	export struct Rect
	{
		float top;
		float bottom;
		float left;
		float right;
	};

	namespace PositionType
	{
		export struct Position
		{
			static constexpr int type_id = 1;
		};

		export struct BorderDistance
		{
			static constexpr int type_id = 2;
		};

		export struct ScreenRatio
		{
			static constexpr int type_id = 3;
		};
	}
	
	export template<typename From, typename To>
		requires (!std::is_same_v<From, To>)
	Rect PositionRectCast(const Rect& src, float area_width, float area_height);

	export template<>
	Rect PositionRectCast<PositionType::Position, PositionType::BorderDistance>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top,
			area_height - src.bottom,
			src.left,
			area_width - src.right
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::BorderDistance, PositionType::Position>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top,
			area_height - src.bottom,
			src.left,
			area_width - src.right
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::Position, PositionType::ScreenRatio>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top / area_height,
			src.bottom / area_height,
			src.left / area_width,
			src.right / area_width
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::ScreenRatio, PositionType::Position>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top * area_height,
			src.bottom * area_height,
			src.left * area_width,
			src.right * area_width
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::BorderDistance, PositionType::ScreenRatio>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top / area_height,
			(area_height - src.bottom) / area_height,
			src.left / area_width,
			(area_width - src.right) / area_width
		};
	}

	export template<>
	Rect PositionRectCast<PositionType::ScreenRatio, PositionType::BorderDistance>(const Rect& src, float area_width, float area_height)
	{
		return {
			src.top * area_height,
			area_height - src.bottom * area_height,
			src.left * area_width,
			area_width - src.right * area_width
		};
	}
	
	template<typename PosType>
	struct _PositionRectBase : public Rect
	{
		using type = PosType;
		
		_PositionRectBase() = default;
		_PositionRectBase(const _PositionRectBase<PosType>&) = default;
		_PositionRectBase(const Rect& r) : Rect(r) {}

		template<typename PosType2>
		_PositionRectBase(const _PositionRectBase<PosType2>& other, float area_width, float area_height)
		{
			auto& [top, bottom, left, right] = PositionRectCast<PosType2, PosType>(other, area_width, area_height);
		}
	};

	export template<typename PosType>
	struct PositionRect : public _PositionRectBase<PosType>
	{
		// These using declarations are to solve the problem that the compiler can't find identifiers from the base class.
		using typename _PositionRectBase<PosType>::type;
		using _PositionRectBase<PosType>::top;
		using _PositionRectBase<PosType>::bottom;
		using _PositionRectBase<PosType>::left;
		using _PositionRectBase<PosType>::right;
		
		PositionRect() = default;
		PositionRect(const PositionRect&) = default;
		PositionRect(const Rect& r) : _PositionRectBase<PosType>(r) {}

		template<typename PosType2>
		PositionRect<PosType2> CastTo(float area_width, float area_height) const
		{
			return PositionRectCast<type, PosType2>(*this, area_width, area_height);
		}

		bool CheckCollisionWith(const PositionRect<type>& other)
		{
			bool horizontal = other.left > right || other.right < left;
			bool vertical = other.top > bottom || other.bottom < top;
			return !horizontal && !vertical;
		}
	};

	export template<>
	struct PositionRect<PositionType::BorderDistance> : public _PositionRectBase<PositionType::BorderDistance>
	{
		PositionRect() = default;
		PositionRect(const PositionRect&) = default;
		PositionRect(const Rect& r) : _PositionRectBase<type>(r) {}

		template<typename PosType2>
		PositionRect<PosType2> CastTo(float area_width, float area_height) const
		{
			return PositionRectCast<type, PosType2>(*this, area_width, area_height);
		}

		bool CheckCollisionWith(const PositionRect<type>& other, float area_width, float area_height) const
		{
			bool horizontal = other.left > area_width - right || area_width - other.right < left;
			bool vertical = other.top > area_height - bottom || area_height - other.bottom < top;
			return !horizontal && !vertical;
		}
	};
}
