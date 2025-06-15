export module ACFT.Optional;

import <memory>;
import <exception>;

export struct NullOptType {};

export constexpr NullOptType NullOpt;

export class OptionalInvalid : std::exception
{
	const char* what() const override
	{
		return "Trying to dereference or get from an invalid Optional.";
	}
};

export template<typename T>
class Optional
{
public:
	Optional(T& obj) noexcept
		: m_Valid(true)
	{
		Fill(obj);
	}

	Optional(T&& obj) noexcept
		: m_Valid(true)
	{
		Fill(std::move(obj));
	}

	Optional(const NullOptType& nullopt) noexcept
	{
		if (m_Valid)
			DestroyCurrentObject();

		m_Valid = false;
	}

	Optional(const Optional& other) noexcept
	{ 
		CopyFrom(other);
	}

	Optional(Optional&& other) noexcept
	{
		CopyFrom(std::move(other));
	}

	~Optional()
	{
		DestroyCurrentObject();
	}

	Optional& operator=(T& obj) noexcept
	{
		Fill(obj);
		return *this;
	}

	Optional& operator=(T&& obj) noexcept
	{
		Fill(std::move(obj));
		return *this;
	}

	Optional& operator=(const NullOptType& nullopt) noexcept
	{
		if (m_Valid)
			DestroyCurrentObject();

		m_Valid = false;
		return *this;
	}

	Optional& operator=(const Optional& other) noexcept
	{
		CopyFrom(other);
		return *this;
	}

	Optional& operator=(Optional&& other) noexcept
	{
		CopyFrom(std::move(other));
		return *this;
	}

	bool IsValid() const noexcept
	{
		return m_Valid;
	}

	T& Get()
	{
		if (!m_Valid)
		{
			throw OptionalInvalid{};
		}

		return *reinterpret_cast<T*>(m_Object);
	}

	T* TryGet() noexcept
	{
		return m_Valid ? reinterpret_cast<T*>(m_Object) : nullptr;
	}

	T& operator*()
	{
		if (!m_Valid)
		{
			throw OptionalInvalid{};
		}

		return *reinterpret_cast<T*>(m_Object);
	}

	T* operator->()
	{
		if (!m_Valid)
		{
			throw OptionalInvalid{};
		}

		return reinterpret_cast<T*>(m_Object);
	}

private:
	void DestroyCurrentObject() noexcept
	{
		std::destroy_at(reinterpret_cast<T*>(m_Object));
	}

	template<typename U>
		requires std::constructible_from<T, U>
	void Fill(U&& obj) noexcept
	{
		if (m_Valid)
			DestroyCurrentObject();
		
		std::construct_at(reinterpret_cast<T*>(m_Object), std::forward<U>(obj));
		m_Valid = true;
	}

	void CopyFrom(const Optional& other) noexcept
	{
		if (other.m_Valid)
			Fill(*reinterpret_cast<T*>(other.m_Object));
		else
		{
			DestroyCurrentObject();
			m_Valid = false;
		}
	}

	void CopyFrom(Optional&& other) noexcept
	{
		if (other.m_Valid)
		{
			if constexpr (std::is_move_constructible_v<T>)
			{
				Fill(std::move(*reinterpret_cast<T*>(other.m_Object)));
			}
			else
			{
				Fill(*reinterpret_cast<T*>(other.m_Object));
			}

			other.m_Valid = false;
		}
		else
		{
			DestroyCurrentObject();
			m_Valid = false;
		}
	}

private:
	alignas(T) unsigned char m_Object[sizeof(T)];
	bool m_Valid;
};
