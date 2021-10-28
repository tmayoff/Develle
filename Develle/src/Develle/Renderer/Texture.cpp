#include <Develle/Renderer/Texture.hpp>

#include <Platform/OpenGL/OpenGLTexture.hpp>

namespace Develle {

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
  // TODO(tyler) Multiple APIs
  return CreateRef<OpenGLTexture2D>(width, height);
}

Ref<Texture2D> Texture2D::Create(const std::string &path) {
  // TODO(tyler) Multiple APIs
  return CreateRef<OpenGLTexture2D>(path);
}

} // namespace Develle
