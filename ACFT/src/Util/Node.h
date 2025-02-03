#ifndef ACFT_NODE_H_
#define ACFT_NODE_H_

namespace ACFT
{
	template<typename T>
	struct AtomicNode
	{
		Ref<T> item;
		std::atomic<AtomicNode*> next;

		AtomicNode()
			: item(nullptr), next(nullptr) 
		{}
		
		AtomicNode(const Ref<T>& _item)
			: item(_item), next(nullptr)
		{}
	};
}

#endif
