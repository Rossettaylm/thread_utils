#ifndef __THREAD_GUARD__H__
#define __THREAD_GUARD__H__

#include <thread>

namespace rossetta {

class ThreadGuard {
public:
  ThreadGuard() = default;

  template <typename Func, typename... Args>
  ThreadGuard(Func &&func, Args &&...args) {
    t = std::thread(std::forward<Func>(func), std::forward<Args>(args)...);
  }

  explicit ThreadGuard(std::thread &&_thread) { t = std::move(_thread); }

  // move constructor
  ThreadGuard(ThreadGuard &&other) { t = std::move(other.t); }

  // move assignment
  ThreadGuard &operator=(ThreadGuard &&other) {
    if (this != &other) {
      t = std::move(other.t);
    }
    return *this;
  }

  virtual ~ThreadGuard() {
    if (t.joinable()) {
      t.join();
    }
  }

  ThreadGuard(const ThreadGuard &other) = delete;
  ThreadGuard &operator=(const ThreadGuard &other) = delete;

private:
  std::thread t;
};

} // namespace rossetta

#endif //!__THREAD_GUARD__H__