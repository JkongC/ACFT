module;

#include <source_location>
#include <string_view>

export module TypeInfo;

export struct keep_prefix{};
export struct no_prefix{};

export template<typename T>
concept TypePrefixType = std::is_same_v<T, keep_prefix> || std::is_same_v<T, no_prefix>;

export template<typename T, TypePrefixType P = keep_prefix>
constexpr std::string_view GetTypeName()
{
#if defined (_MSC_VER)
	std::string_view whole = std::source_location::current().function_name();
	auto start = whole.find("GetTypeName");
	auto end = whole.rfind(">");
	if constexpr (std::is_same_v<P, keep_prefix>)
		return std::string_view{ whole.begin() + start + sizeof("GetTypeName"), whole.begin() + end };
	else
	{
		std::string_view with_prefix{ whole.begin() + start + sizeof("GetTypeName"), whole.begin() + end };
		
		if (with_prefix.find("class") != with_prefix.npos)
			with_prefix.remove_prefix(sizeof("class"));
		else if (with_prefix.find("struct") != with_prefix.npos)
			with_prefix.remove_prefix(sizeof("struct"));
			
		return with_prefix;
	}
#else
	static_assert(false, "This function is not enabled for current compiler.");
#endif
}