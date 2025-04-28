module;

#include "Platform.h"

export module RefCount;

import <unordered_map>;
import <mutex>;

import UUID;

namespace ACFT
{
	export class ACFT_API RefCount
	{
	protected:
		RefCount();
		~RefCount();

	protected:
		UUID m_Identifier;
	};

	class RefCountManager
	{
	public:
		static void AddRef(const UUID& ref);
		static void IncrementRef(const UUID& ref);
		static void DecrementRef(const UUID& ref);

	private:
		RefCountManager();
		RefCountManager(const RefCountManager&) = delete;
		RefCountManager(RefCountManager&&) = delete;
		RefCountManager& operator=(const RefCountManager&) = delete;

	private:
		static inline std::unordered_map<UUID, size_t> s_Manager;
		static inline std::mutex s_Mtx;
	};
}
