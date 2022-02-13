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
  return SDL_BUTTON(state) == button;
}

glm::vec2 Input::GetMousePosition() {
  int x = 0, y = 0;
  SDL_GetMouseState(&x, &y);
  return {x, y};
}

float Input::GetMouseX() { return GetMousePosition().x; }

float Input::GetMouseY() { return GetMousePosition().y; }

}  // namespace Develle
