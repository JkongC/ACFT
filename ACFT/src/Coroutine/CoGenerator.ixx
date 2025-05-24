export module ACFT.CoGenerator;

import <coroutine>;

namespace ACFT::Coroutine
{
	export template<typename T>
	struct Generator
	{
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		handle_type handle;

		struct promise_type
		{
			T value;

			Generator<T> get_return_object() { return handle_type::from_promise(*this); }
			std::suspend_always initial_suspend() noexcept { return {}; }
			std::suspend_always final_suspend() noexcept { return {}; }
			void return_void() {}
			void unhandled_exception() {}

			template<std::convertible_to<T> From>
			std::suspend_always yield_value(From&& val)
			{
				value = std::forward<From>(val);
				return {};
			}
		};

		Generator(handle_type _h) : handle(_h) {}
		~Generator() { if (handle) handle.destroy(); }

		explicit operator bool()
		{
			fill();
			return !handle.done();
		}

		T operator()()
		{
			fill();
			filled = false;
			return std::move(handle.promise().value);
		}

	private:
		bool filled = false;

		void fill()
		{
			if (!filled)
			{
				handle.resume();
				filled = true;
			}
		}
	};
}