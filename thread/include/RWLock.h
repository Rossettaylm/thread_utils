#ifndef RWLOCK_H
#define RWLOCK_H

#include <vector>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <memory>

#define DEBUG_WR_LOCK 1

class RWLock
{
public:
 void read_lock();

 void print_debug(const char *msg) const;

 void write_lock();

 void read_release();

 void write_release();

private:
 std::condition_variable m_write_cv;
 std::condition_variable m_read_cv;
 int32_t m_read_count{0};
 int32_t m_write_wait_count{0};
 bool m_write_own{false};
 std::mutex m_mutex;
};

#endif