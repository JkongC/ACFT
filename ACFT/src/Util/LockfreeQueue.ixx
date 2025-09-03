export module LockfreeQueue;

import <type_traits>;
import <optional>;
import <atomic>;
import Log;
import Types;

namespace ACFT
{
	export template<typename T>
	struct AtomicRefNode
	{
		Ref<T> item;
		std::atomic<AtomicRefNode<T>*> next;

		AtomicRefNode()
			: item(nullptr), next(nullptr)
		{
		}

		AtomicRefNode(const Ref<T>& _item)
			: item(_item), next(nullptr)
		{
		}
	};

	export template<typename T>
	struct AtomicScopeNode
	{
		Scope<T> item;
		std::atomic<AtomicScopeNode<T>*> next;

		AtomicScopeNode()
			: item(nullptr), next(nullptr)
		{
		}

		AtomicScopeNode(Scope<T>&& _item)
			: item(std::move(_item)), next(nullptr)
		{
		}
	};
	
	export enum QueueNodeType
	{
		scope = 0, ref
	};

	export enum class QueueRejectStrategy
	{
		dispose_all, dispose_head
	};
	
	export template<typename T, QueueNodeType NodeType = scope, QueueRejectStrategy Str = QueueRejectStrategy::dispose_all, size_t MaxSize = 0>
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
			{
				if constexpr (Str == QueueRejectStrategy::dispose_all)
					return;
				else if (Str == QueueRejectStrategy::dispose_head)
					Pop();
			}
			
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
			{
				if constexpr (Str == QueueRejectStrategy::dispose_all)
					return;
				else if (Str == QueueRejectStrategy::dispose_head)
					Pop();
			}

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
							m_Size++;
							return;
						}
					}
					else
					{
						tail.compare_exchange_weak(old_tail, old_next);
					}
				}
			}
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
							m_Size--;
							return std::move(result);
						}
					}
				}
			}
		}

	private:
		std::atomic<InnerNodeType*> head;
		std::atomic<InnerNodeType*> tail;
		std::atomic<size_t> m_Size;
	};
}