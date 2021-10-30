#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

namespace Develle {

class Timer {
public:
  Timer() { Reset(); }

  void Reset() { start = std::chrono::high_resolution_clock::now(); }

  float Elapsed() {
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::high_resolution_clock::now() - start)
        .count();
  }

  float ElapsedMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - start)
        .count();
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

} // namespace Develle
#endif // TIMER_HPP_
