#ifndef INPUT_HPP_
#define INPUT_HPP_

#include <glm/glm.hpp>

#include "KeyCodes.hpp"
#include "MouseCodes.hpp"

namespace Develle {

class Input {
 public:
  static bool IsKeyPressed(KeyCode key);

  static bool IsMouseButtonPressed(MouseCode button);
  static glm::vec2 GetMousePosition();
  static float GetMouseX();
  static float GetMouseY();
};

}  // namespace Develle
#endif  // INPUT_HPP_
