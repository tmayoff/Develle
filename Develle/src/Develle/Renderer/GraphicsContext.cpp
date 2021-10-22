#include <Develle/Renderer/GraphicsContext.h>

#include <Platform/OpenGL/OpenGLContext.h>

namespace Develle {

Scope<GraphicsContext> GraphicsContext::Create(void *window) {
  // TODO(tyler) Fill this out with the RendererAPI things

  return CreateScope<OpenGLGraphicsContext>(static_cast<SDL_Window *>(window));
}

} // namespace Develle
