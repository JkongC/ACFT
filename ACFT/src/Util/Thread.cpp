module ACFT.Thread;

import Log;

namespace ACFT
{
	Thread::Thread(const std::function<void()>& thread_func)
		: m_Func(thread_func)
	{ }

	Scope<Thread>& ThreadManager::GetThread(std::string_view& thread_name)
	{
		static Scope<Thread> null_thread = nullptr;
		
		if (auto it = ThreadManager::s_Threads.find(thread_name);
			it != ThreadManager::s_Threads.end())
		{
			return it->second;
		}
		else
		{
			ACFT_LOG_WARN("Trying to get an unexisted thread \"{}\"!", thread_name);
			return null_thread;
		}
	}
	
	void ThreadManager::CreateThread(std::string_view thread_name, const std::function<void()>& thread_func)
	{
		std::lock_guard<std::recursive_mutex> lock(ThreadManager::s_Mtx);

		auto& threads = ThreadManager::s_Threads;
		
		if (auto it = threads.find(thread_name);
			it == threads.end())
		{
			Thread* t = new Thread(thread_func);
			threads.emplace(thread_name, Scope<Thread>(t));
		}
		else
		{
			ACFT_LOG_WARN("Trying to create an existed thread \"{}\"!", thread_name);
		}
	}

	void ThreadManager::StartThread(std::string_view thread_name)
	{
		std::lock_guard<std::recursive_mutex> lock(ThreadManager::s_Mtx);
		
		if (auto& t = GetThread(thread_name))
		{
			t->m_Thread = MakeScope<std::thread>(t->m_Func);
		}
	}

	void ThreadManager::JoinThread(std::string_view thread_name)
	{
		std::lock_guard<std::recursive_mutex> lock(ThreadManager::s_Mtx);

		if (auto& t = GetThread(thread_name))
		{
			t->m_Thread->join();
		}

		RemoveThread(thread_name);
	}

	bool ThreadManager::IsCurrentThread(std::string_view thread_name)
	{
		std::lock_guard<std::recursive_mutex> lock(ThreadManager::s_Mtx);

		if (auto& t = GetThread(thread_name))
		{
			return t->m_Thread->get_id() == std::this_thread::get_id();
		}

		return false;
	}

	bool ThreadManager::IsThreadDetached(std::string_view thread_name)
	{
		std::lock_guard<std::recursive_mutex> lock(ThreadManager::s_Mtx);

		if (auto& t = GetThread(thread_name))
		{
			return t->m_Detached;
		}

		return false;
	}

	void ThreadManager::RemoveThread(std::string_view thread_name)
	{
		if (auto it = ThreadManager::s_Threads.find(thread_name);
			it != ThreadManager::s_Threads.end())
		{
			ThreadManager::s_Threads.erase(it);
		}
	}
}
