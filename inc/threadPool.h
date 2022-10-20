#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <iostream>
#include <asio.hpp>


class ThreadPool {
public:
	ThreadPool(size_t threads_count) {
		pool_ = std::make_unique<asio::thread_pool>(threads_count);
	};
	ThreadPool() {
		pool_ = std::make_unique<asio::thread_pool>();
	};
	~ThreadPool() {
		join();
	};
	
	void join() {
		pool_->join();
	};

	template <typename F, typename... Args>
	auto pushTask(F task, Args... args) {
		size_t desc = id++;
		using type = typename std::result_of<F(Args...)>::type;
		auto p_task = std::packaged_task<type(void)>(std::bind(task, std::forward<Args...>(args)...));
		auto future = p_task.get_future();
		free_tasks_[id] = SyncState();
		auto wrapped_func = [p = std::move(p_task), d=desc, this]() mutable {
			setState(d, State::kRunning);
			p();
			setState(d, State::kFinish);
		};
		asio::post(*pool_, std::move(wrapped_func));
		return std::make_pair<size_t, std::future<type>>(std::move(desc), std::move(future));
	}
private:
	enum class State {
		kWaiting,
		kRunning,
		kFinish
	};
	struct SyncState {
		SyncState() {
			sync_ = std::make_unique<std::mutex>();
			State::kWaiting;
		};
		std::unique_ptr<std::mutex> sync_;
		State state_;
	};

	std::unique_ptr<asio::thread_pool> pool_;
	std::unordered_map<size_t, SyncState> free_tasks_;
	size_t id = 0;

	void setState(size_t id, State new_state) {
		auto& [mtx, state]= free_tasks_[id];
		std::lock_guard<std::mutex> lock(*mtx);
		state = new_state;
	}
	
};

#endif // !1
