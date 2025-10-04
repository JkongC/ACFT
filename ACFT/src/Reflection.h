#if not defined(ACFT_REFLECTION)
#define ACFT_REFLECTION

#define CREATE_ENUM(NAME, ...) \
    enum class NAME : size_t { __VA_ARGS__ }; \
    constexpr size_t NAME##_ElementCount = []() constexpr { \
        constexpr std::string_view str = #__VA_ARGS__; \
        size_t count = 1; \
        for (char c : str) { \
            if (c == ',') ++count; \
        } \
        return count; \
    }();

#endif //ACFT_REFLECTION
