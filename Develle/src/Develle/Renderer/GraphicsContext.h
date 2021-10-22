#ifndef GRAPHICSCONTEXT_H_
#define GRAPHICSCONTEXT_H_

namespace Develle {

class GraphicsContext {
public:
  virtual ~GraphicsContext() = default;

  virtual void Init() = 0;
  virtual void SwapBuffers() = 0;

  static Scope<GraphicsContext> Create(void *window);
};

} // namespace Develle
#endif // GRAPHICSCONTEXT_H_
