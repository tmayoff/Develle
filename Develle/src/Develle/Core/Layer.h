#ifndef LAYER_H_
#define LAYER_H_

#include <Develle/Core/Timestep.h>
#include <Develle/Events/Event.h>

namespace Develle {

class Layer {
public:
  Layer(const std::string &name = "Layer");
  virtual ~Layer() = default;

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(Timestep) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Event &) {}

  const std::string &GetName() const { return debugName; }

protected:
  std::string debugName;
};

} // namespace Develle
#endif // LAYER_H_
