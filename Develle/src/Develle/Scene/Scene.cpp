#include "Scene.hpp"

#include <Develle/Renderer/Renderer2D.hpp>

#include "Components.hpp"
#include "Entity.hpp"

namespace Develle {

Scene::Scene() {}
Scene::~Scene() {}

Entity Scene::CreateEntity(const std::string &name) {
  Entity entity{registry.create(), this};
  entity.AddComponent<TransformComponent>();
  auto &tag = entity.AddComponent<TagComponent>();
  tag.Tag = name.empty() ? "Entity" : name;
  return entity;
}

void Scene::DestroyEntity(Entity entity) { registry.destroy(entity); }

void Scene::OnUpdateRuntime(Timestep) {
  // TODO(tyler)
}

void Scene::OnUpdateEditor(Timestep, EditorCamera &camera) {
  Renderer2D::BeginScene(camera);

  auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

  for (auto entity : group) {
    auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
    Renderer2D::DrawQuad(transform.GetTransform(), sprite, (int)entity);
  }

  Renderer2D::EndScene();
}

void Scene::OnViewportResize(uint32_t width, uint32_t height) {
  viewportWidth = width;
  viewportHeight = height;

  auto view = registry.view<CameraComponent>();
  for (auto entity : view) {
    auto &cameraComponent = view.get<CameraComponent>(entity);
    if (!cameraComponent.FixedAspectRatio) cameraComponent.Camera.SetViewportSize(width, height);
  }
}

Entity Scene::GetPrimaryCameraEntity() {
  auto view = registry.view<CameraComponent>();
  for (auto entity : view) {
    const auto &camera = view.get<CameraComponent>(entity);
    if (camera.Primary) return Entity{entity, this};
  }

  DV_CORE_WARN("No Primary camera");
}

}  // namespace Develle
