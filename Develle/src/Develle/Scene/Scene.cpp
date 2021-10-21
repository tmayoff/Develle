#include <Develle/Scene/Components.h>
#include <Develle/Scene/Entity.h>
#include <Develle/Scene/Scene.h>

namespace Develle {

Scene::Scene() {}
Scene::~Scene() {}

Entity Scene::CreateEntity(const std::string &name) {
  Entity entity{registry.create(), this};
  //  TODO(tyler) Add Transform component
  //   entity.AddComponent<TransformComponent>();
  auto &tag = entity.AddComponent<TagComponent>();
  tag.Tag = name.empty() ? "Entity" : name;
  return entity;
}

void Scene::DestroyEntity(Entity entity) { registry.destroy(entity); }

void Scene::OnUpdateRuntime(Timestep) {
  // TODO(tyler)
}

void Scene::OnUpdateEditor(Timestep /*ts, EditorCamera camera*/) {}

template <typename T> void Scene::OnComponentAdded(Entity, T &) {
  //   static_assert(false);
}

template <>
void Scene::OnComponentAdded<TagComponent>(Entity, TagComponent &) {}

} // namespace Develle
