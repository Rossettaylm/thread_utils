#ifndef __SEMAPHORE__H__
#define __SEMAPHORE__H__

#include <condition_variable>
#include <memory>
#include <mutex>

namespace rossetta {

class Semaphore {
public:
  Semaphore(int count = 0) : count_(count) {
    lock_ = std::make_unique<std::mutex>();
    cond_ = std::make_unique<std::condition_variable>();
  }
  ~Semaphore() {}

  Semaphore(Semaphore &&other) noexcept;            // move constructor
  Semaphore &operator=(Semaphore &&other) noexcept; // move assign

public:
  int GetSemCount();
  void Wait();
  void Release();

private:
  int count_;
  std::unique_ptr<std::mutex> lock_;              // 保护count变量
  std::unique_ptr<std::condition_variable> cond_; // 进行线程的唤醒通知
};

} // namespace rossetta

#endif //!__SEMAPHORE__H__