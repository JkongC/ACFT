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
		Thread(Thread&&) = default;

	private:
		friend class ThreadManager;
		
		Thread(bool immediate_start, const std::function<void()>& thread_func);

	private:
		std::thread m_Thread;
		std::atomic<bool> m_Detached = false;
		std::condition_variable m_StartCV;
		std::mutex m_Mtx;
		bool m_Started = false;
	};

	export class ThreadManager
	{
	public:
		static void CreateThread(std::string_view thread_name, bool immediate_start, const std::function<void()>& thread_func);
		static bool IsThreadExisted(const std::string_view& thread_name);
		static void StartThread(std::string_view thread_name);
		static void JoinThread(std::string_view thread_name);
		static void DetachThread(std::string_view thread_name);

		static bool IsCurrentThread(std::string_view thread_name);
		static bool IsThreadDetached(std::string_view thread_name);

		static void RemoveThread(std::string_view thread_name);


	private:
		ThreadManager() = default;
		ThreadManager(const ThreadManager&) = delete;
		ThreadManager& operator=(const ThreadManager&) = delete;

		static Scope<Thread>& GetThread(const std::string_view& thread_name);

	private:
		static inline std::unordered_map<std::string, Scope<Thread>, StringHashFunc, EqualFunc> s_Threads;
		static inline std::recursive_mutex s_Mtx;
	};
}
