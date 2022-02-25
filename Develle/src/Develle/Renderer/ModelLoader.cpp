#include "ModelLoader.hpp"

#include <fstream>

namespace Develle {

auto ModelLoader::LoadMesh(const std::filesystem::path& filepath) -> Mesh {
  std::filesystem::path modelPath = filepath;
  if (!filepath.is_absolute()) {
    modelPath = ASSETS_ROOT / filepath;
  }

  std::string line;
  std::fstream file;
  file.open(modelPath);
  if (!file.is_open()) {
    DV_CORE_WARN("File failed to open");
    return {};
  }

  std::vector<glm::vec3> meshVertices{};
  std::vector<glm::vec2> meshUVs{};
  std::vector<glm::vec3> meshNormals{};
  std::vector<std::vector<std::string>> meshFaces{};
  while (std::getline(file, line)) {
    if (line.starts_with("#")) {
      continue;
    }

    if (line.starts_with("v ")) {
      meshVertices.push_back(processVec3(line));
    }

    if (line.starts_with("vn ")) {
      meshNormals.push_back(processVec3(line));
    }

    if (line.starts_with("vt ")) {
      meshUVs.push_back(processVec2(line));
    }

    if (line.starts_with("f ")) {
      meshFaces.push_back(processFace(line));
    }
  }

  std::vector<MeshVertex> vertices;
  std::vector<uint32_t> indices;
  for (const auto& face : meshFaces) {
    // Indices
    for (uint32_t i = 1; i < face.size() - 1; i++) {
      uint32_t offset = vertices.size();
      indices.push_back(offset + i);
      indices.push_back(offset + i + 1);
      indices.push_back(offset);
    }

    // Vertices
    for (const auto& v : face) {
      // position/uv/normal
      std::vector<std::string> faceComponents = split(v, '/');
      int positionIndex = std::stoi(faceComponents[0]) - 1;
      int uvIndex = std::stoi(faceComponents[1]) - 1;
      int normalIndex = std::stoi(faceComponents[2]) - 1;

      MeshVertex mv{};
      mv.Position = glm::vec4(meshVertices.at(positionIndex), 1.0F);
      mv.TexCoord = meshUVs.at(uvIndex);
      mv.Normal = meshNormals.at(normalIndex);
      mv.Color = {1.0F, 1.0F, 1.0F, 1.0F};

      vertices.push_back(mv);
    }
  }

  return {vertices, indices};
}

auto ModelLoader::processVec3(const std::string& line) -> glm::vec3 {
  auto elems = split(line, ' ');
  elems.erase(elems.begin());
  DV_CORE_ASSERT(elems.size() == 3, "Vertex doesn't contain 3 values");

  return glm::vec3{std::stof(elems[0]), std::stof(elems[1]), std::stof(elems[2])};
}

auto ModelLoader::processVec2(const std::string& line) -> glm::vec2 {
  auto elems = split(line, ' ');
  elems.erase(elems.begin());
  DV_CORE_ASSERT(elems.size() == 2, "Vertex doesn't contain 2 values");

  return glm::vec2{std::stof(elems[0]), std::stof(elems[1])};
}

auto ModelLoader::processFace(const std::string& line) -> std::vector<std::string> {
  std::string faceString = line.substr(2);

  return {split(faceString, ' ')};
}

auto ModelLoader::split(const std::string& s, char delimiter) -> std::vector<std::string> {
  std::vector<std::string> elems;
  split(s, delimiter, std::back_inserter(elems));
  return elems;
}

}  // namespace Develle
