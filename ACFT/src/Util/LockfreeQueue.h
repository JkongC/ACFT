#ifndef ACFT_LOCKFREEQUEUE_H_
#define ACFT_LOCKFREEQUEUE_H_

#include "Node.h"

namespace ACFT
{
	template<typename T>
	class LockfreeQueue
	{
	public:
		LockfreeQueue()
		{
			AtomicNode<T>* dummy = new AtomicNode<T>();
			head.store(dummy);
			tail.store(dummy);
		}

		~LockfreeQueue()
		{
			while (AtomicNode<T>* old_head = head.load())
			{
				head.store(old_head->next.load());
				delete old_head;
			}
		}

		void Push(const Ref<T>& item)
		{
			AtomicNode<T>* new_node = new AtomicNode<T>(item);
			AtomicNode<T>* old_tail = nullptr;
			AtomicNode<T>* old_next = nullptr;

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
							command_count++;
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

		std::optional<Ref<T>> Pop()
		{
			AtomicNode<T>* old_head = nullptr;
			AtomicNode<T>* old_tail = nullptr;
			AtomicNode<T>* next = nullptr;

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
						Ref<T>& result = next->item;
						if (head.compare_exchange_weak(old_head, next))
						{
							delete old_head;
							command_count--;
							return std::move(result);
						}
					}
				}
			}
		}

	private:
		std::atomic<AtomicNode<T>*> head;
		std::atomic<AtomicNode<T>*> tail;

		std::atomic<int> command_count = 0;
	};
}

#endif