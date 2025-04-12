export module LockfreeQueue;

import <type_traits>;
import <optional>;
import Node;

namespace ACFT
{
	export template<typename T, template <typename> typename NodeType = Scope>
		requires std::is_same_v<NodeType<T>, Ref<T>> || std::is_same_v<NodeType<T>, Scope<T>>
	class LockfreeQueue
	{
		using InnerNodeType = std::conditional_t<
			std::is_same_v<NodeType<T>, Ref<T>>,
			AtomicRefNode<T>,
			AtomicScopeNode<T>>;
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

		template<typename T, typename... Args>
		void Emplace(Args&&... args)
		{
			if constexpr (std::is_same_v<NodeType<T>, Ref<T>>)
			{
				Push(MakeRef<T>(std::forward<Args...>(args)...));
			}
			else
			{
				Push(std::move(MakeScope<T>(std::forward<Args...>(args)...)));
			}
		}

		void Push(NodeType<T> item)
		{
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
		}

		std::optional<NodeType<T>> Pop()
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
						NodeType<T>& result = next->item;
						if (head.compare_exchange_weak(old_head, next))
						{
							delete old_head;
							return std::move(result);
						}
					}
				}
			}
		}

	private:
		std::atomic<InnerNodeType*> head;
		std::atomic<InnerNodeType*> tail;
	};
}