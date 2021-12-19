#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <entt.hpp>

#include "Scene.hpp"

namespace Develle {

class Entity {
 public:
  Entity() = default;
  Entity(entt::entity handle, Scene *scene);
  Entity(const Entity &other) = default;

  template <typename T, typename... Args>
  T &AddComponent(Args &&...args) {
    DV_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");

    T &component = scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
    // scene->OnComponentAdded<T>(*this, component);
    return component;
  }

  template <typename T>
  T &GetComponent() {
    DV_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
    return scene->registry.get<T>(entityHandle);
  }

  template <typename T>
  bool HasComponent() {
    return scene->registry.all_of<T>(entityHandle);
  }

  template <typename T>
  void RemoveComponent() {
    DV_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");
    scene->registry.remove<T>(entityHandle);
  }

  operator bool() const { return entityHandle != entt::null; }
  operator entt::entity() const { return entityHandle; }
  operator uint32_t() const { return (uint32_t)entityHandle; }

  bool operator==(const Entity &other) const {
    return entityHandle == other.entityHandle && scene == other.scene;
  }

  bool operator!=(const Entity &other) const { return !(*this == other); }

 private:
  entt::entity entityHandle{entt::null};
  Scene *scene = nullptr;
};

}  // namespace Develle

#endif  // ENTITY_HPP_
