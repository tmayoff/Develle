#ifndef IMGUILAYER_H_
#define IMGUILAYER_H_

#include <Develle/Core/Layer.h>

namespace Develle {

class ImGuiLayer : public Layer {
public:
  ImGuiLayer();
  ~ImGuiLayer() = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnEvent(Event &e) override;

  void Begin();
  void End();

  void BlockEvents(bool block) { blockEvents = block; }

private:
  bool blockEvents = true;
};

} // namespace Develle
#endif // IMGUILAYER_H_
