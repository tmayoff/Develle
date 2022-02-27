#pragma once
#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include <glm/glm.hpp>

namespace Develle {

class Light {
 public:
  Light() = default;
  Light(const glm::vec3 &position, const glm::vec3 &color) : position(position), color(color) {}

  const glm::vec3 &GetPosition() const { return position; }
  const glm::vec3 &GetColor() const { return color; }

 private:
  glm::vec3 position;
  glm::vec3 color;
};

}  // namespace Develle
#endif  // LIGHT_HPP_
