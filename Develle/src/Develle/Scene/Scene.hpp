#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <Develle/Core/Timestep.hpp>
#include <Develle/Renderer/EditorCamera.hpp>
#include <entt.hpp>

namespace Develle {

class Entity;

class Scene {
 public:
  Scene();
  ~Scene();

  Entity CreateEntity(const std::string &name = std::string());
  void DestroyEntity(Entity entity);

  void OnUpdateRuntime(Timestep delta);
  void OnUpdateEditor(Timestep delta, EditorCamera &camera);
  void OnViewportResize(uint32_t width, uint32_t height);

  Entity GetPrimaryCameraEntity();

 private:
  template <typename T>
  void OnComponentAdded(Entity entity, T &component);

  entt::registry registry;
  uint32_t viewportWidth = 0, viewportHeight = 0;

  friend class Entity;
  friend class SceneSerializer;
  friend class SceneHierarchyPanel;
};

}  // namespace Develle

#endif  // SCENE_HPP_
