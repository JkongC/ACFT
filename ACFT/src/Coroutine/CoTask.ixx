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
			std::conditional_t<std::is_same_v<T, void>, std::nullptr_t, T> result;

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
			
			template<std::convertible_to<T> From>
			void return_value(From&& val)
				requires !std::is_same_v<T, void>
			{
				result = std::forward<From>(val);
			}
		};

		void operator()() const
		{
			handle.resume();
		}

		explicit operator bool() const
		{
			return !handle.done();
		}

		Task(handle_type h) : handle(h) {}
		~Task() { if (handle) handle.destroy(); }

	protected:
		handle_type handle;
	};

	export template<typename T>
	struct TimestepTask
	{
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct promise_type
		{
			T result;

			float time_step;
			bool time_step_updated = false;

			Task<T> get_return_object() { return handle_type::from_promise(*this); }
			std::suspend_always initial_suspend() noexcept { return {}; }

			auto final_suspend() noexcept
			{
				struct FinalAwaitableWithResult
				{
					T res;

					bool await_ready() const { return false; }
					bool await_suspend(std::coroutine_handle<>) { return false; }
					T await_resume() const
					{
						return std::move(res);
					}
				};

				return FinalAwaitableWithResult{ std::move(result) };
			}

			template<std::convertible_to<T> From>
			void return_value(From&& val)
				requires !std::is_same_v<T, void>
			{
				result = std::forward<From>(val);
			}

			void unhandled_exception() {}
		};

		void operator()(float time_step)
		{
			auto& promise = handle.promise();
			promise.time_step = time_step;
			promise.time_step_updated = true;
			handle.resume();
		}

		operator bool() const
		{
			return handle.done();
		}

		struct TimestepAwaitable
		{
			promise_type* promise;
			
			bool await_ready() const { return false; }
			void await_suspend(std::coroutine_handle<promise_type> h) 
			{
				promise = &h.promise();
				promise->time_step_updated = false;
			}
			float await_resume() const 
			{
				return promise->time_step;
			}
		};

		TimestepTask(handle_type _h) : handle(_h) {}
		~TimestepTask() { }

	private:
		handle_type handle;
	};

	export template<>
	struct TimestepTask<void>
	{
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct promise_type
		{
			float time_step;
			bool time_step_updated = false;

			TimestepTask<void> get_return_object() { return handle_type::from_promise(*this); }
			std::suspend_always initial_suspend() noexcept { return {}; }
			std::suspend_always final_suspend() noexcept { return {}; }

			void return_void() {}

			void unhandled_exception() {}
		};

		void operator()(float time_step)
		{
			auto& promise = handle.promise();
			promise.time_step = time_step;
			promise.time_step_updated = true;
			handle.resume();
		}

		operator bool() const
		{
			return handle.done();
		}

		struct TimestepAwaitable
		{
			promise_type* promise;

			bool await_ready() const { return false; }
			void await_suspend(std::coroutine_handle<promise_type> h)
			{
				promise = &h.promise();
				promise->time_step_updated = false;
			}
			float await_resume() const
			{
				return promise->time_step;
			}
		};

		TimestepTask(handle_type _h) : handle(_h) {}
		~TimestepTask() { if (handle && !handle.done()) handle.destroy(); }

	private:
		handle_type handle;
	};
}