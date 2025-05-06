export module ACFT.Thread;

import <thread>;
import <functional>;
import <unordered_map>;
import <string>;
import <mutex>;

import Types;

namespace ACFT
{
	class ThreadManager;

	export class Thread
	{
	public:
		~Thread() = default;

	private:
		friend class ThreadManager;
		
		Thread(const std::function<void()>& thread_func);

	private:
		Scope<std::thread> m_Thread = nullptr;
		std::function<void()> m_Func;
		std::atomic<bool> m_Detached = false;
	};

	export class ThreadManager
	{
	public:
		static void CreateThread(std::string_view thread_name, const std::function<void()>& thread_func);
		static void StartThread(std::string_view thread_name);
		static void JoinThread(std::string_view thread_name);

		static bool IsCurrentThread(std::string_view thread_name);
		static bool IsThreadDetached(std::string_view thread_name);

		static void RemoveThread(std::string_view thread_name);


	private:
		ThreadManager() = default;
		ThreadManager(const ThreadManager&) = delete;
		ThreadManager& operator=(const ThreadManager&) = delete;

		static Scope<Thread>& GetThread(std::string_view& thread_name);

	private:
		static inline std::unordered_map<std::string, Scope<Thread>, StringHash, StringEqual> s_Threads;
		static inline std::recursive_mutex s_Mtx;
	};
}
