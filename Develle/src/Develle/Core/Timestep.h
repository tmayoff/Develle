#ifndef TIMESTEP_H_
#define TIMESTEP_H_

namespace Develle {

class Timestep {
public:
  Timestep(float time = 0.0f) : time(time) {}

  operator float() const { return time; }

  float GetSeconds() const { return time / 1000.0f; }
  float GetMilliseconds() const { return time; }

private:
  float time;
};

} // namespace Develle
#endif // TIMESTEP_H_
