export module TemplateHelper.Variant;

import <variant>;
import <functional>;

import TemplateHelper.Type;

namespace TemplateHelper::Variant
{
	using namespace TemplateHelper::Type;

	export template<typename T, SpecializationOf<std::variant> Var>
		bool IsType(Var&& variant)
	{
		return std::get_if<T>(&variant) != nullptr;
	}

	export template<typename T, SpecializationOf<std::variant> Var>
		T ValueOr(Var&& variant, const T& alternative)
	{
		T* result = std::get_if<T>(&variant);
		if (result)
			return *result;
		else
			return alternative;
	}

	export template<typename T, SpecializationOf<std::variant> Var>
		T ValueOrDo(Var&& variant, const T& alternative, std::function<void()> reaction)
	{
		T* result = std::get_if<T>(&variant);
		if (result)
			return *result;
		else
		{
			reaction();
			return alternative;
		}
	}

	export template<typename T, SpecializationOf<std::variant> Var>
		T& Value(Var&& variant) noexcept
	{
		return std::get<T>(variant);
	}
}
