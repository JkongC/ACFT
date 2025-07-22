export module ACFT.ObjectPool;

import <vector>;
import <atomic>;
import <shared_mutex>;

import Memory;
import Log;

namespace ACFT
{	
	struct _Default_Extend_Strategy
	{
		static size_t Extend(size_t old)
		{
			return static_cast<size_t>(old * 1.5);
		}
	};

	using half_extend = _Default_Extend_Strategy;

	export template<typename T>
	concept PoolExtendStrategy = requires(size_t old) {
		{ T::Extend(old) } -> std::convertible_to<size_t>;
	};

	export enum class ObjectPoolMode
	{
		single_threaded, multi_threaded
	};
	
	export template<typename T, ObjectPoolMode mode = ObjectPoolMode::single_threaded, PoolExtendStrategy Ext = _Default_Extend_Strategy>
	class ObjectPool
	{
	public:
		using value_type = std::decay_t<std::remove_reference_t<T>>;
		using flags_type = std::conditional_t<mode == ObjectPoolMode::single_threaded, std::vector<bool>, std::vector<unsigned char>>;

		static constexpr size_t DefaultCapacity = 1000;

		ObjectPool()
			: m_MemoryPool(DefaultCapacity)
			, m_PoolCapacity(DefaultCapacity)
			, m_OccupyFlag(DefaultCapacity, false)
		{ }

		ObjectPool(size_t capacity)
			: m_MemoryPool(capacity)
			, m_PoolCapacity(capacity)
			, m_OccupyFlag(capacity, false)
		{ }

		value_type* allocate(size_t n)
		{
			if (n > 1)
			{
				throw std::bad_alloc();
			}

			if (n == 0)
			{
				return nullptr;
			}

			if constexpr (mode == ObjectPoolMode::multi_threaded)
			{
				for (size_t i = 0; i < m_OccupyFlag.size(); i++)
				{
					std::atomic_ref<unsigned char> flag(m_OccupyFlag.at(i));
					unsigned char expected = 0;
					if (flag.compare_exchange_strong(expected,
						1, 
						std::memory_order_acq_rel, 
						std::memory_order_acquire))
					{
						return GetMemoryPoint(i);
					}
				}
			}
			else
			{
				for (size_t i = 0; i < m_OccupyFlag.size(); i++)
				{
					if (!m_OccupyFlag.at(i))
					{
						m_OccupyFlag.at(i) = true;
						return GetMemoryPoint(i);
					}
				}
			}
			
			return static_cast<T*>(::operator new(sizeof(T)));
		}

		void deallocate(value_type* ptr, size_t n)
		{
			if (n > 1)
			{
				throw std::bad_alloc{};
			}

			if (n == 0)
			{
				return;
			}

			auto [in_pool, idx] = IsInPool(ptr);
			if (in_pool)
			{
				if constexpr (mode == ObjectPoolMode::multi_threaded)
				{
					std::atomic_ref<unsigned char> flag(m_OccupyFlag.at(idx));
				 	flag.store(0, std::memory_order_release);
				}
				else
				{
					m_OccupyFlag.at(idx) = false;
				}
			}
			else
			{
				::operator delete(ptr);
			}
		}

		template<typename U>
		struct rebind
		{
			using other = std::allocator<U>;
		};

	private:
		struct Chunk
		{
			value_type* start;
			size_t capacity;

			Chunk(size_t capacity)
				: start(
					static_cast<T*>(::operator new(capacity * sizeof(value_type), std::align_val_t{ alignof(T) }))
				)
				, capacity(capacity)
			{ }

			Chunk(const Chunk&) = delete;
			
			Chunk(Chunk&& chunk) noexcept
				: start(chunk.start), capacity(chunk.capacity)
			{
				chunk.start = nullptr;
				chunk.capacity = 0;
			}

			~Chunk()
			{
				if (start != nullptr)
					::operator delete(static_cast<void*>(start), std::align_val_t{alignof(T)});

				start = nullptr;
				capacity = 0;
			}
		};

		value_type* GetMemoryPoint(size_t idx)
		{
			if (m_MemoryPool.capacity >= idx + 1)
			{
				return m_MemoryPool.start + idx;
			}

			return nullptr;
		}

		std::pair<bool, size_t> IsInPool(T* ptr)
		{
			if (ptr - m_MemoryPool.start >= 0 && m_MemoryPool.start + m_MemoryPool.capacity - ptr >= 0)
			{
				return { true, (ptr - m_MemoryPool.start) };
			}

			return { false, 0 };
		}

	private:
		Chunk m_MemoryPool;
		size_t m_PoolCapacity;
		flags_type m_OccupyFlag;
	};

	export template<typename T, ObjectPoolMode mode = ObjectPoolMode::single_threaded, PoolExtendStrategy Ext = _Default_Extend_Strategy>
	using RefObjectPool = ObjectPool<MemoryTraitType<typename RefInplaceBlockTraits<T>::type>, mode, Ext>;
}