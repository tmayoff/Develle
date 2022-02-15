#include "Entity.hpp"

namespace Develle {

Entity::Entity(entt::entity handle, Scene *scene) : entityHandle(handle), scene(scene) {}
Entity::Entity(const Entity &other) : entityHandle(other.entityHandle), scene(other.scene) {}

}  // namespace Develle
