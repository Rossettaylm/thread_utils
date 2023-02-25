#include "Semaphore.h"
#include "ThreadGuard.h"
#include "util.h"
#include <iostream>

using namespace std;
using namespace rossetta;

Semaphore sem;
std::mutex m;
int count_func1 = 0, count_func2 = 0;

void Func1(string msg) {
  while (true) {
    sem.Wait(); // 等待func2释放信号之后才会执行
    m.lock();
    cout << msg << endl;
    count_func1++;
    m.unlock();
    SleepForSeconds(1);
  }
}

void Func2(string msg) {
  while (true) {
    m.lock();
    cout << msg << endl;
    m.unlock();
    count_func2++;
    sem.Release();
    SleepForSeconds(1);
  }
}

int main() {
  ThreadGuard t1(Func1, "func1: daraole");
  constexpr int thread_num = 10;
  ThreadGuard t[thread_num];
  for (int i = 0; i < thread_num; ++i) {
    t[i] = ThreadGuard(Func2, string()
                                  .append("func")
                                  .append(to_string(i))
                                  .append(": ")
                                  .append("queshiniubi"));
  }

  while (true) {
    if (count_func1 > 20) {
      m.lock();
      cout << count_func1 << ' ' << count_func2 << endl;
      m.unlock();
      abort();
    }
  }

  return 0;
}