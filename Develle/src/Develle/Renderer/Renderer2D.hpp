#ifndef RENDERER2D_HPP_
#define RENDERER2D_HPP_

#include <Develle/Renderer/OrthographicCamera.hpp>

namespace Develle {

class Renderer2D {
public:
  static void Init();
  static void Shutdown();

  static void
  BeginScene(const OrthographicCamera &camera); // TODO(tyler) Remove
  static void EndScene();
  static void Flush();

  struct Statistics {
    uint32_t DrawCalls = 0;
    uint32_t QuadCount = 0;

    uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
    uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
  };

  static void ResetStats();
  static Statistics GetStats();

private:
  static void StartBatch();
  static void NextBatch();
};

} // namespace Develle
#endif // RENDERER2D_HPP_
