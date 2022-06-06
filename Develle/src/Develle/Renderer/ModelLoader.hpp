#pragma once
#ifndef MODELLOADER_HPP_
#define MODELLOADER_HPP_

#include "Mesh.hpp"

namespace Develle {

class ModelLoader {
 public:
  static auto LoadMesh(const std::filesystem::path& filepath) -> Mesh;

 private:
  static auto processVec3(const std::string& line) -> glm::vec3;
  static auto processVec2(const std::string& line) -> glm::vec2;
  static auto processFace(const std::string& line) -> std::vector<std::string>;

  template <typename Out>
  static auto split(const std::string& s, char delimiter, Out out);
  static auto split(const std::string& s, char delimiter) -> std::vector<std::string>;
};

template <typename Out>
inline auto ModelLoader::split(const std::string& s, char delimiter, Out out) {
  std::istringstream iss(s);
  std::string item;
  while (std::getline(iss, item, delimiter)) {
    *out++ = item;
  }
}

}  // namespace Develle
#endif  // MODELLOADER_HPP_
