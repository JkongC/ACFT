export module Timer;

import <chrono>;
import <functional>;

export namespace ACFT
{
	export using nanosecond = std::nano;
	export using millisecond = std::milli;
	export using second = std::ratio<1, 1>;

	template<typename type, typename period>
	concept Duration_Convertible = std::is_arithmetic_v<type> && std::_Is_ratio_v<period>;

	template<typename Type = float, typename Period = millisecond>
		requires Duration_Convertible<Type, Period>
	class NormalTimer
	{
	public:
		NormalTimer()
			: m_Last(std::chrono::steady_clock::now())
		{ }

		Type GetElapsed()
		{
			auto elapsed = std::chrono::steady_clock::now() - m_Last;
			return std::chrono::duration_cast<std::chrono::duration<Type, Period>>(elapsed).count();
		}

		void Flush()
		{
			m_Last = std::chrono::steady_clock::now();
		}

		Type GetElapsedAndFlush()
		{
			auto now = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::duration<Type, Period>>(now - m_Last);
			m_Last = now;
			return elapsed.count();
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_Last;
	};

	template<typename Type = float, typename Period = millisecond>
		requires Duration_Convertible<Type, Period>
	class ScopedTimer
	{
	public:
		ScopedTimer() = delete;

		ScopedTimer(std::function<void(Type)> callback)
			: m_Callback(callback)
		{
			this->m_Start = std::chrono::high_resolution_clock::now();
		}

		~ScopedTimer()
		{
			auto now = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::duration<Type, Period>>(now - this->m_Start);
			this->m_Callback(elapsed.count());
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		std::function<void(Type)> m_Callback;
	};
}
