#include <Develle/Renderer/Texture.hpp>

#include <Platform/OpenGL/OpenGLTexture.hpp>

namespace Develle {

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
  // TODO(tyler) Multiple APIs
  return CreateRef<OpenGLTexture2D>(width, height);
}

Ref<Texture2D> Texture2D::Create(std::string path) {
  auto p = std::filesystem::path(path);
  if (!p.is_absolute())
    path = std::string(ASSETS_ROOT) + std::string("/") + path;

  // TODO(tyler) Multiple APIs
  return CreateRef<OpenGLTexture2D>(path);
}

} // namespace Develle
