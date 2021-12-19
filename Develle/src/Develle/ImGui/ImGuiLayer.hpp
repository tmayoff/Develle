#ifndef IMGUILAYER_H_
#define IMGUILAYER_H_

#include <Develle/Core/Layer.hpp>
#include <Develle/Events/ApplicationEvent.hpp>
#include <Develle/Events/KeyEvent.hpp>
#include <Develle/Events/MouseEvent.hpp>

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
  bool OnKeyPressedEvent(KeyPressedEvent &e);
  bool OnKeyReleasedEvent(KeyReleasedEvent &e);
  bool OnKeyTypedEvent(KeyTypedEvent &e);
  bool OnMouseButtonPressedEvent(MouseButtonPressedEvent &e);
  bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &e);
  bool OnMouseScrolledEvent(MouseScrolledEvent &e);

  bool blockEvents = true;
};

}  // namespace Develle
#endif  // IMGUILAYER_H_
