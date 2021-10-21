#ifndef TIMESTEP_H_
#define TIMESTEP_H_

namespace Develle {

class Timestep {
  Timestep(float time = 0.0f) : time(time) {}

  operator float() const { return time; }

  float GetSeconds() const { return time; }
  float GetMilliseconds() const { return time * 1000.0f; }

private:
  float time;
};

} // namespace Develle
#endif // TIMESTEP_H_
