#include <Develle/Renderer/Framebuffer.hpp>

// #include <Platform/OpenGL/OpenGLFramebuffer.hpp>

namespace Develle {

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec) {
  // TODO(tyler) Multiple APIs
  //   return CreateRef<OpenGLFramebuffer>(spec);
  return nullptr;
}

} // namespace Develle
