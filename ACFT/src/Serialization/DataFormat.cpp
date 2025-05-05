module;

#include <mutex>
#include <fstream>

module ACFT.DataFormat;

import Log;

namespace ACFT::DataFormat
{	
	Ref<ObjDataFile> ObjDataFile::Open(const std::filesystem::path& file)
	{
		return MakeRef<ObjDataFile>(file);
	}
	
	ObjDataFile::ObjDataFile(const std::filesystem::path& file)
		: m_Path(file)
	{	
		// std::fstream can't create a new file if using std::ios::in.
		// Thus I use std::ofstream to create if the file doesn't exist.
		if (!std::filesystem::exists(file))
		{
			std::ofstream(file, std::ios::out).close();
		}

		m_FStream.open(file, std::ios::binary | std::ios::out | std::ios::in);
		if (!m_FStream)
		{
			ACFT_LOG_WARN("Failed to load file \"{}\"!", file.string().c_str());
			return;
		}

		m_FStream.seekg(0);
		m_FStream.seekp(0);
	}

	Ref<Buffer> ObjDataFile::ReadObj()
	{
		std::lock_guard<std::mutex> lock(m_Mtx);
		
		size_t size = 0;
		m_FStream.read(reinterpret_cast<char*>(&size), sizeof(size_t));

		Ref<Buffer> buffer = MakeRef<Buffer>(size);
		m_FStream.read(reinterpret_cast<char*>(buffer->GetRawData()), size);

		return buffer;
	}

	void ObjDataFile::WriteObj(Ref<Buffer> data)
	{
		std::lock_guard<std::mutex> lock(m_Mtx);
		
		size_t size = data->GetSize();
		m_FStream.write(reinterpret_cast<char*>(&size), sizeof(size_t));

		m_FStream.write(reinterpret_cast<char*>(data->GetRawData()), size);
	}

	bool ObjDataFile::IsReadable()
	{
		return m_FStream.peek() != std::ifstream::traits_type::eof();
	}
}
