export module TemplateHelper.MetaGen;

import <array>;

namespace TemplateHelper::MetaGen
{
	export template<size_t... Idxes>
	struct IndexSequence
	{
		static constexpr size_t size = sizeof...(Idxes);
		
		static constexpr auto ToArray()
		{
			return std::array<size_t, size>{ Idxes... };
		}
	};

	/**
	 * Make an index sequence of [BeginIdx, EndIdx).
	 */
	export template<size_t BeginIdx, size_t EndIdx, size_t... Idxes>
		requires (BeginIdx < EndIdx)
	struct MakeIndexSequence : MakeIndexSequence<BeginIdx, EndIdx - 1, EndIdx - 1, Idxes> {};

	template<size_t BeginIdx, size_t... Idxes>
	struct MakeIndexSequence<BeginIdx, BeginIdx, Idxes...> : IndexSequence<Idxes> {};

	/**
	 * Make an index sequence of [0, EndIdx).
	 */
	export template<size_t EndIdx>
	struct MakeTrivialIndexSequence : MakeIndexSequence<0, EndIdx> {};

	export template<size_t... Idxes1, size_t... Idxes2>
	consteval auto ConcatIndexSequence(IndexSequence<Idxes1...> seq1, IndexSequence<Idxes2...> seq2)
	{
		return IndexSequence<Idxes1..., Idxes2...>{};
	}
	
	template<size_t BeginIdx, size_t EndIdx, size_t ThrowIdx>
	using _SequenceAfterThrown = decltype([](size_t bg, size_t ed, size_t th) consteval {
		if constexpr (bg == th)
			return MakeIndexSequence<bg + 1, ed>{};
		else if constexpr (ed == th + 1)
			return MakeIndexSequence<bg, th>{};
		else
			return ConcatIndexSequence(MakeIndexSequence<bg, th>{}, MakeIndexSequence<th + 1, ed>{});
	}(BeginIdx, EndIdx, ThrowIdx));
	
	export template<size_t BeginIdx, size_t EndIdx, size_t ThrowIdx>
		requires (ThrowIdx >= BeginIdx && ThrowIdx < EndIdx)
	struct MakeIndexSequenceWithout : _SequenceAfterThrown<BeginIdx, EndIdx, ThrowIdx> {};
}
