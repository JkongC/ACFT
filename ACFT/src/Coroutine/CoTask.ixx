export module ACFT.CoTask;

import <coroutine>;

namespace ACFT::Coroutine
{
	export template<typename T = void>
	struct Task
	{
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct promise_type
		{
			T result;

			Task<T> get_return_object() { return handle_type::from_promise(*this); }
			std::suspend_always initial_suspend() noexcept { return {}; }

			auto final_suspend() noexcept
			{
				if constexpr (std::is_same_v<T, void>)
				{
					return std::suspend_never{};
				}
				else
				{
					struct FinalAwaitableWithResult
					{
						T res;

						bool await_ready() const { return false; }
						void await_suspend(std::coroutine_handle<>) {}
						T await_resume() const
						{
							return std::move(res);
						}
					};

					return FinalAwaitableWithResult{ std::move(result) };
				}
			}

			void return_void()
			{
				static_assert(std::is_same_v<T, void>, "Task<T> Must return a value, use Task<> or Task instead.");
			}
			
			template<std::convertible_to<T> From>
			void return_value(From&& val)
				requires !std::is_same_v<T, void>
			{
				result = std::forward<From>(val);
			}
		};

		Task(handle_type h) : handle(h) {}
		~Task() { if (handle) handle.destroy(); }

	private:
		handle_type handle;
	};
}