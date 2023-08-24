#include "Semaphore.h"
#include "ThreadGuard.h"
#include "util.h"
#include <string>
#include <iostream>
#include <atomic>
#include "RWLock.h"

using namespace std;
using namespace rossetta;
using std::atomic_bool;

Semaphore sem;
std::mutex m;
int count_func1 = 0, count_func2 = 0;
atomic_bool flag;

void Func1(string msg) {
  while (!flag) {
    sem.Wait(); // 等待func2释放信号之后才会执行
    std::unique_lock<std::mutex> ul(m);
    cout << msg << endl;
    ul.unlock();
    count_func1++;
    SleepForSeconds(1);
  }
}

void Func2(string msg) {
  while (!flag) {
    std::unique_lock<std::mutex> ul(m);
    cout << msg << endl;
    ul.unlock();
    count_func2++;
    sem.Release();
    SleepForSeconds(1);
  }
}

int main() {
  ThreadGuard t1(Func1, "func1: daraole");
  constexpr int thread_num = 2;
  ThreadGuard t[thread_num];
  for (int i = 0; i < thread_num; ++i) {
    t[i] = ThreadGuard(Func2, string()
                                  .append("func")
                                  .append(to_string(i))
                                  .append(": ")
                                  .append("queshiniubi"));
  }

  while (true) {
    if (count_func1 > 10) {
      m.lock();
      cout << count_func1 << ' ' << count_func2 << endl;
      m.unlock();
      break;
    }
  }

  flag.store(true);

  return 0;
}