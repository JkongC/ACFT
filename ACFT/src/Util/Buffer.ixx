export module ACFT.Buffer;

import Log;

namespace ACFT
{
	export class Buffer
	{
	public:
		Buffer(size_t size)
			: m_Size(size), m_Data(new char[size])
			, m_WritePos(m_Data), m_ReadPos(m_Data)
		{ }

		~Buffer()
		{
			delete[] m_Data;
		}

		size_t GetSize() const
		{
			return reinterpret_cast<size_t>(m_WritePos) - reinterpret_cast<size_t>(m_Data);
		}

		size_t GetMaxSize() const
		{
			return m_Size;
		}

		void* GetRawData()
		{
			return m_Data;
		}

		void* GetReadPos()
		{
			return m_ReadPos;
		}

		void* GetWritePos()
		{
			return m_WritePos;
		}

		/**
		 * Directly write binary data into the buffer.
		 * 
		 * \param data The data that can be directly written.
		 * \return Whether the operation is successful. False if the buffer is out of space.
		 */
		template<typename T>
		bool WriteRaw(const T& data)
		{
			if (reinterpret_cast<char*>(m_WritePos) + sizeof(T) > reinterpret_cast<char*>(m_Data) + m_Size)
				return false;

			*reinterpret_cast<T*>(m_WritePos) = data;
			m_WritePos = reinterpret_cast<void*>(reinterpret_cast<char*>(m_WritePos) + sizeof(T));

			return true;
		}

		template<typename T>
		T ReadRaw()
		{
			if (reinterpret_cast<char*>(m_ReadPos) + sizeof(T) > reinterpret_cast<char*>(m_Data) + m_Size)
				return T{};

			T* data = reinterpret_cast<T*>(m_ReadPos);
			m_ReadPos = reinterpret_cast<void*>(reinterpret_cast<char*>(m_ReadPos) + sizeof(T));

			return *data;
		}

	private:
		void* m_Data;
		size_t m_Size;
		void* m_WritePos;
		void* m_ReadPos;
	};
}
