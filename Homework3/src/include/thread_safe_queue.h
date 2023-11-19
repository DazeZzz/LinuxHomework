#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T> class ThreadSafeQueue {
private:
  mutable std::mutex mut_;
  std::queue<std::shared_ptr<T>> data_queue_;
  std::condition_variable data_condition_;

public:
  void WaitAndPop(T &value) {
    std::lock_guard<std::mutex> lock{mut_};
    data_condition_.wait(lock, [this] { return !data_queue_.empty(); });
    value = std::move(*data_queue_.front());
    data_queue_.pop();
  }

  std::shared_ptr<T> WaitAndPop() {
    std::lock_guard<std::mutex> lock{mut_};
    data_condition_.wait(lock, [this] { return !data_queue_.empty(); });
    std::shared_ptr<T> temp = data_queue_.front();
    data_queue_.pop();
    return temp;
  }

  bool TryPop(T &value) {
    std::lock_guard<std::mutex> lock{mut_};
    if (data_queue_.empty()) {
      return false;
    }
    value = std::move(*data_queue_.front());
    data_queue_.pop();
    return true;
  }

  std::shared_ptr<T> TryPop() {
    std::lock_guard<std::mutex> lock{mut_};
    if (data_queue_.empty()) {
      return {};
    }
    std::shared_ptr<T> temp = data_queue_.front();
    data_queue_.pop();
    return temp;
  }

  void Push(T new_value) {
    std::shared_ptr<T> data{std::make_shared<T>(std::move(new_value))};
    std::lock_guard<std::mutex> lock{mut_};
    data_queue_.push(data);
    data_condition_.notify_one();
  }

  bool Empty() const {
    std::lock_guard<std::mutex> lock{mut_};
    return data_queue_.empty();
  }
};