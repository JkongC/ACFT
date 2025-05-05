export module ACFT.Codec;

import <functional>;

import Types;
export import ACFT.Buffer;

namespace ACFT
{
	export template<typename T>
	class Codec
	{
	public:
		constexpr Codec(std::function<Ref<Buffer>(const T&)> encoder, std::function<T(Ref<Buffer>)> decoder)
			: m_Encoder(encoder), m_Decoder(decoder)
		{ }

		Ref<Buffer> Encode(const T& obj) const
		{
			return m_Encoder(obj);
		}

		T Decode(Ref<Buffer> data) const
		{
			return m_Decoder(data);
		}

	protected:
		std::function<Ref<Buffer>(const T&)> m_Encoder;
		std::function<T(Ref<Buffer>)> m_Decoder;
	};

	export namespace Codecs
	{
		template<typename T>
		const Codec<T> PLAIN_CODEC {
			[](const T& obj) -> Ref<Buffer>
			{
				Ref<Buffer> buf = MakeRef<Buffer>(sizeof(T));
				buf->WriteRaw<T>(obj);
				return buf;
			},
			[](Ref<Buffer> buf) -> T
			{
				return buf->ReadRaw<T>();
			}
		};
	}
}
