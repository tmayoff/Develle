#ifndef SCENESERIALIZER_HPP_
#define SCENESERIALIZER_HPP_

#include <Develle/Scene/Scene.h>

namespace Develle {

class SceneSerializer {
public:
  SceneSerializer(const Ref<Scene> &scene);

  void Serialize(const std::string &filepath);
  void SerializeRuntime(const std::string &filepath);

  bool Deserialize(const std::string &filepath);
  bool DeserializeRuntime(const std::string &filepath);

private:
  Ref<Scene> scene;
};

} // namespace Develle
#endif // SCENESERIALIZER_HPP_
