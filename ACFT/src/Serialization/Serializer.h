#ifndef ACFT_SERIALIZER_H_
#define ACFT_SERIALIZER_H_

namespace ACFT
{
	template<typename _Ty>
	concept Serializable = requires(_Ty obj)
	{
		[] <typename... Args>(_Ty t) -> decltype(t.Serialize(std::declval<Args>...)) {}(obj);
		[] <typename... Args>(_Ty t) -> decltype(t.Unserialize(std::declval<Args>...)) {}(obj);
	};
	
	template<typename _Ty>
	void WritePlain(_Ty& obj, std::ofstream& out)
		requires std::is_trivial_v<_Ty> && std::is_standard_layout_v<_Ty>
	{
		out.write(reinterpret_cast<const char*>(&obj), sizeof(_Ty));
	}

	template<typename _Ty>
	void ReadPlain(_Ty& obj, std::ifstream& in)
		requires std::is_trivial_v<_Ty>&& std::is_standard_layout_v<_Ty>
	{
		in.read(reinterpret_cast<char*>(&obj), sizeof(_Ty));
	}
}

#endif