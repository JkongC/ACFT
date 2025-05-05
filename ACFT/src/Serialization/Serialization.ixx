export module ACFT.Serialization;

import <fstream>;

import Types;
export import ACFT.Codec;
export import ACFT.DataFormat;

namespace ACFT::Serialization
{
	export template<typename T>
	class DataProvider
	{
	public:
		DataProvider(const T& data)
			: m_HasData(true), m_DataRet(data)
		{ }

		DataProvider()
			: m_HasData(false), m_DataRet()
		{ }

		DataProvider(DataProvider&& dp)
			: m_HasData(dp.m_HasData), m_DataRet(std::move(dp.m_DataRet))
		{ }

		DataProvider& Or(const T& alternative)
		{
			if (!m_HasData)
				m_DataRet = alternative;

			return *this;
		}

		operator T() const
		{
			return m_DataRet;
		}

	private:
		bool m_HasData;
		T m_DataRet;
	};
	
	export template<typename T>
	void Serialize(const T& obj, const Codec<T>& codec, Ref<DataFormat::ObjDataFile> file)
	{
		file->WriteObj(codec.Encode(obj));
	}

	export template<typename T>
	DataProvider<T> Deserialize(const Codec<T>& codec, Ref<DataFormat::ObjDataFile> file)
	{
		if (file->IsReadable())
			return codec.Decode(file->ReadObj());
		else
			return {};
	}
}
