export module LockfreeQueue;

import <type_traits>;
import <optional>;
import Node;

namespace ACFT
{
	export enum QueueNodeType
	{
		scope = 0, ref
	};
	
	export template<typename T, QueueNodeType NodeType = scope, size_t MaxSize = 0>
	class LockfreeQueue
	{
		using InnerNodeType = std::conditional_t<
			NodeType == ref,
			AtomicRefNode<T>,
			AtomicScopeNode<T>>;

		using PtrType = std::conditional_t<
			NodeType == ref,
			Ref<T>,
			Scope<T>>;

	public:
		LockfreeQueue()
		{
			InnerNodeType* dummy = new InnerNodeType();
			head.store(dummy);
			tail.store(dummy);
		}

		~LockfreeQueue()
		{
			while (InnerNodeType* old_head = head.load())
			{
				head.store(old_head->next.load());
				delete old_head;
			}
		}

		size_t GetSize()
		{
			return m_Size;
		}

		size_t GetMaxSize()
		{
			return MaxSize;
		}

		template<typename T, typename... Args>
		void Emplace(Args&&... args)
		{
			if (MaxSize != 0 && m_Size >= MaxSize)
				return;
			
			if constexpr (std::is_same_v<InnerNodeType, AtomicRefNode<T>>)
			{
				Push(MakeRef<T>(std::forward<Args...>(args)...));
			}
			else
			{
				Push(std::move(MakeScope<T>(std::forward<Args...>(args)...)));
			}

			m_Size++;
		}

		void Push(const PtrType& item)
		{
			if (MaxSize != 0 && m_Size >= MaxSize)
				return;

			InnerNodeType* new_node = new InnerNodeType(std::move(item));
			InnerNodeType* old_tail = nullptr;
			InnerNodeType* old_next = nullptr;

			while (true)
			{
				old_tail = tail.load();
				old_next = old_tail->next.load();

				if (old_tail == tail.load())
				{
					if (old_next == nullptr)
					{
						if (old_tail->next.compare_exchange_weak(old_next, new_node))
						{
							tail.compare_exchange_weak(old_tail, new_node);
							return;
						}
					}
					else
					{
						tail.compare_exchange_weak(old_tail, old_next);
					}
				}
			}

			m_Size++;
		}

		std::optional<PtrType> Pop()
		{
			InnerNodeType* old_head = nullptr;
			InnerNodeType* old_tail = nullptr;
			InnerNodeType* next = nullptr;

			while (true)
			{
				old_head = head.load();
				old_tail = tail.load();
				next = old_head->next.load();

				if (old_head == head.load())
				{
					if (old_head == old_tail)
					{
						if (next == nullptr)
							return std::nullopt;

						tail.compare_exchange_weak(old_tail, next);
					}
					else
					{
						PtrType& result = next->item;
						if (head.compare_exchange_weak(old_head, next))
						{
							delete old_head;
							return std::move(result);
						}
					}
				}
			}

			m_Size--;
		}

	private:
		std::atomic<InnerNodeType*> head;
		std::atomic<InnerNodeType*> tail;
		std::atomic<size_t> m_Size;
	};
}