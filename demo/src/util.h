#ifndef __UTIL__H__
#define __UTIL__H__

#include <thread>
#include <chrono>

inline void SleepForSeconds(int seconds) {
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

#endif  //!__UTIL__H__



