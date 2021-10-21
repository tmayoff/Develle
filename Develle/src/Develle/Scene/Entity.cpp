#include <Develle/Scene/Entity.h>

namespace Develle {

Entity::Entity(entt::entity handle, Scene *scene)
    : entityHandle(handle), scene(scene) {}
} // namespace Develle
