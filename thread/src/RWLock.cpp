#include "RWLock.h"

/**
 * @brief 打印调试信息
 *
 * @param msg
 */
void RWLock::print_debug(const char *msg) const {
#if DEBUG_WR_LOCK
  auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
  std::cout << timestamp << "," << msg << ",read count:" << m_read_count
            << ",write own:" << std::boolalpha << m_write_own
            << ",write wait count:" << m_write_wait_count << std::endl;
#endif
}


/**
 * @brief 写锁: 当未拥有其他写锁和未拥有其他读锁的时候进行锁定
 */
void RWLock::write_lock() {
  std::unique_lock<std::mutex> lock(m_mutex);
  m_write_wait_count++;
  m_write_cv.wait(
      lock, [this]() { return !this->m_write_own && this->m_read_count == 0; });
  m_write_own = true;
  m_write_wait_count--;
  print_debug("write_lock");
}


/**
 * @brief 写锁释放: 释放写锁拥有全owns_write, 同时根据优先统治其他写锁，后通知读锁
 */
void RWLock::write_release() {
  std::unique_lock<std::mutex> lock(m_mutex);
  m_write_own = false;
  if (m_write_wait_count >= 1) {
    m_write_cv.notify_one();
  } else {
    m_read_cv.notify_all();
  }
  print_debug("write_release");
}

/**
 * @brief 写锁锁定：等待没有写锁占用且无写锁等待时，进行锁定
 */
void RWLock::read_lock() {
  std::unique_lock<std::mutex> lock(m_mutex);
  m_read_cv.wait(lock, [this]() {
    return !this->m_write_own && this->m_write_wait_count == 0;
  });

  m_read_count++;
  print_debug("read_lock");
}


/**
 * @brief 读锁释放：写锁可能会因此阻塞，判断是否是否通知写锁
 */
void RWLock::read_release() {
  std::unique_lock<std::mutex> lock(m_mutex);
  --m_read_count;
  print_debug("read_release");
  if (m_read_count == 0 && m_write_wait_count > 0) {
    m_write_cv.notify_one();
  }
}