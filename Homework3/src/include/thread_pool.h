#include "join_threads.h"
#include "thread_safe_queue.h"
#include <atomic>
#include <functional>
#include <future>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadPool {
public:
  ThreadPool() : done_(false), joiner_(threads_) {
    unsigned const thread_count = std::thread::hardware_concurrency();
    try {
      for (unsigned i = 0; i < thread_count; ++i) {
        threads_.emplace_back(&ThreadPool::WorkerThread, this);
      }
    } catch (...) {
      done_ = true;
      throw;
    }
  }

  ThreadPool(ThreadPool &other) = delete;
  ThreadPool(ThreadPool &&other) = delete;
  ThreadPool &operator=(const ThreadPool &other) = delete;
  ThreadPool &operator=(ThreadPool &&other) = delete;

  ~ThreadPool() { done_ = true; }

  template <typename FunctionType>
  std::future<std::result_of_t<FunctionType()>> SubmitTask(FunctionType fun) {
    using result_type = std::result_of_t<FunctionType()>;
    std::packaged_task<void()> task{std::move(fun)};
    std::future<result_type> res{task.get_future()};
    tasks_.Push(std::move(task));
    return res;
  }

private:
  std::atomic_bool done_;
  ThreadSafeQueue<std::packaged_task<void()>> tasks_;
  std::vector<std::thread> threads_;
  JoinThreads joiner_;

  void WorkerThread() {
    while (!done_) {
      std::packaged_task<void()> task;
      if (tasks_.TryPop(task)) {
        task();
      } else {
        std::this_thread::yield();
      }
    }
  }
};