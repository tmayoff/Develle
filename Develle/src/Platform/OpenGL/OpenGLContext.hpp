#ifndef OPENGLCONTEXT_HPP_
#define OPENGLCONTEXT_HPP_

#include <Develle/Renderer/GraphicsContext.hpp>

struct SDL_Window;
typedef void *SDL_GLContext;

namespace Develle {

class OpenGLGraphicsContext : public GraphicsContext {
 public:
  OpenGLGraphicsContext(SDL_Window *window);

  void Init() override;
  void SwapBuffers() override;

 private:
  SDL_Window *windowHandle;
  SDL_GLContext context;
};

}  // namespace Develle

#endif  // OPENGLCONTEXT_HPP_
