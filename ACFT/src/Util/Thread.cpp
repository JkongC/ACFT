module ACFT.Thread;

import Log;

namespace ACFT
{
	Thread::Thread(bool immediate_start, const std::function<void()>& thread_func)
		: m_Thread([this, thread_func]() -> void
	{
		std::unique_lock<std::mutex> lock(m_Mtx);
		m_StartCV.wait(lock, [this] {return m_Started;});
		thread_func();
	})
	{
		if (immediate_start)
		{
			m_Started = true;
			m_StartCV.notify_all();
		}
	}

	bool ThreadManager::IsThreadExisted(const std::string_view& thread_name)
	{
		return ThreadManager::s_Threads.find(thread_name) != ThreadManager::s_Threads.end();
	}

	Scope<Thread>& ThreadManager::GetThread(const std::string_view& thread_name)
	{
		static Scope<Thread> null_thread{nullptr};
		
		if (auto it = ThreadManager::s_Threads.find(thread_name);
			it != ThreadManager::s_Threads.end())
		{
			return it->second;
		}
		else
		{
			return null_thread;
		}
	}
	
	void ThreadManager::CreateThread(std::string_view thread_name, bool immediate_start, const std::function<void()>& thread_func)
	{
		std::lock_guard<std::recursive_mutex> lock(ThreadManager::s_Mtx);

		auto& threads = ThreadManager::s_Threads;
		
		if (auto it = threads.find(thread_name);
			it == threads.end())
		{
			Thread* t = new Thread(immediate_start, thread_func);
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
			if (!t->m_Started)
			{
				t->m_Started = true;
				t->m_StartCV.notify_all();
			}
		}
	}

	void ThreadManager::JoinThread(std::string_view thread_name)
	{
		std::lock_guard<std::recursive_mutex> lock(ThreadManager::s_Mtx);

		if (auto& t = GetThread(thread_name))
		{
			if (!t->m_Detached)
				t->m_Thread.join();
		}

		RemoveThread(thread_name);
	}

	void ThreadManager::DetachThread(std::string_view thread_name)
	{
		std::lock_guard<std::recursive_mutex> lock(ThreadManager::s_Mtx);

		if (auto& t = GetThread(thread_name))
		{
			t->m_Thread.detach();
		}
	}

	bool ThreadManager::IsCurrentThread(std::string_view thread_name)
	{
		std::lock_guard<std::recursive_mutex> lock(ThreadManager::s_Mtx);

		if (auto& t = GetThread(thread_name))
		{
			return t->m_Thread.get_id() == std::this_thread::get_id();
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
