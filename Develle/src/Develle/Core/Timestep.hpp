#ifndef TIMESTEP_HPP_
#define TIMESTEP_HPP_

namespace Develle {

class Timestep {
 public:
  Timestep(float time = 0.0f) : time(time * 0.001f) {}

  operator float() const { return time; }

  float GetSeconds() const { return time; }
  float GetMilliseconds() const { return time * 1000.0f; }

 private:
  float time;
};

}  // namespace Develle

#endif  // TIMESTEP_HPP_
