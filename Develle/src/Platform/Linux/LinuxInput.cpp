#include <SDL2/SDL.h>

#include <Develle/Core/Input.hpp>

namespace Develle {

bool Input::IsKeyPressed(const KeyCode key) {
  int numKeys = 0;
  auto state = SDL_GetKeyboardState(&numKeys);
  return state[key];
}

bool Input::IsMouseButtonPressed(const MouseCode button) {
  auto state = SDL_GetMouseState(nullptr, nullptr);
  MouseCode mouseButton;
  switch (SDL_BUTTON(state)) {
    case SDL_BUTTON_LEFT:
      return button == MouseCode::ButtonLeft;
    case SDL_BUTTON_MIDDLE:
      return button == MouseCode::ButtonMiddle;
    case SDL_BUTTON_RIGHT:
      return button == MouseCode::ButtonRight;
  }
  return false;
}

glm::vec2 Input::GetMousePosition() {
  int x = 0, y = 0;
  SDL_GetMouseState(&x, &y);
  return {x, y};
}

float Input::GetMouseX() { return GetMousePosition().x; }

float Input::GetMouseY() { return GetMousePosition().y; }

}  // namespace Develle
