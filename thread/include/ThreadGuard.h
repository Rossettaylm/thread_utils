#ifndef __THREAD_GUARD__H__
#define __THREAD_GUARD__H__

#include <thread>

namespace rossetta {

class ThreadGuard {
public:
  template <typename Func, typename... Args>
  explicit ThreadGuard(Func &&func, Args &&...args) {
    t = std::thread(std::forward<Func>(func), std::forward<Args>(args)...);
  }

  explicit ThreadGuard(std::thread &&_thread) : t(std::move(_thread)) {  }

  // move constructor
  ThreadGuard(ThreadGuard &&other)  noexcept : t(std::move(other.t)) {  }

  // move assignment
  ThreadGuard &operator=(ThreadGuard &&other)  noexcept {
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