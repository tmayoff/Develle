#ifndef OPENGLCONTEXT_H_
#define OPENGLCONTEXT_H_

#include <Develle/Renderer/GraphicsContext.h>

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

} // namespace Develle
#endif // OPENGLCONTEXT_H_
