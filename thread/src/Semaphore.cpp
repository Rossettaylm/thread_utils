#include "Semaphore.h"

namespace rossetta {

int Semaphore::GetSemCount() {
  std::lock_guard<std::mutex> lg(*lock_);
  return count_;
}

/**
 * @brief 等待有资源时，就进行消耗
 */
void Semaphore::Wait() {
  std::unique_lock<std::mutex> lg(*lock_);
  cond_->wait(lg, [=] { return count_ > 0; });
  count_--;
}

/**
 * @brief 释放资源
 */
void Semaphore::Release() {
  std::unique_lock<std::mutex> lg(*lock_);
  count_++;
  cond_->notify_one();
}

/**
 * @brief move constructor
 * @param other
 */
Semaphore::Semaphore(Semaphore &&other) noexcept {
  count_ = other.count_;
  lock_ = std::move(other.lock_);
  cond_ = std::move(other.cond_);
}

/**
 * @brief move assignment
 *
 * @param other
 * @return Semaphore&
 */
Semaphore &Semaphore::operator=(Semaphore &&other) noexcept {
  // self assignment check
  if (this != &other) {
    count_ = other.count_;
    lock_ = std::move(other.lock_);
    cond_ = std::move(other.cond_);
  }
  return *this;
}

} // namespace rossetta