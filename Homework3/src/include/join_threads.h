#include <thread>
#include <vector>

class JoinThreads {
private:
  std::vector<std::thread> &threads_;

public:
  explicit JoinThreads(std::vector<std::thread> &thread) : threads_(thread){};

  JoinThreads(JoinThreads &other) = delete;
  JoinThreads(JoinThreads &&other) = delete;
  JoinThreads &operator=(const JoinThreads &other) = delete;
  JoinThreads &operator=(const JoinThreads &&other) = delete;

  ~JoinThreads() {
    for (auto &thread : threads_) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }
};