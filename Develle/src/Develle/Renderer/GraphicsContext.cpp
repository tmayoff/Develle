#include "GraphicsContext.hpp"

#include <Platform/OpenGL/OpenGLContext.hpp>

namespace Develle {

Scope<GraphicsContext> GraphicsContext::Create(void *window) {
  // TODO(tyler) Fill this out with the RendererAPI things

  return CreateScope<OpenGLGraphicsContext>(static_cast<SDL_Window *>(window));
}

}  // namespace Develle
