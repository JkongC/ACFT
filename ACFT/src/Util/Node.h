#ifndef ACFT_NODE_H_
#define ACFT_NODE_H_

namespace ACFT
{
	template<typename T>
	struct AtomicRefNode
	{
		Ref<T> item;
		std::atomic<AtomicRefNode<T>*> next;

		AtomicRefNode()
			: item(nullptr), next(nullptr) 
		{}
		
		AtomicRefNode(const Ref<T>& _item)
			: item(_item), next(nullptr)
		{}
	};

	template<typename T>
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
}

#endif
