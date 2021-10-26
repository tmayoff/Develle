#include <Platform/OpenGL/OpenGLRendererAPI.h>

#include <glad/glad.h>

namespace Develle {

void OpenGLMessageCallback(unsigned, unsigned, unsigned, unsigned severity, int,
                           const char *message, const void *) {
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    DV_CORE_FATAL(message);
    return;
  case GL_DEBUG_SEVERITY_MEDIUM:
    DV_CORE_ERROR(message);
    return;
  case GL_DEBUG_SEVERITY_LOW:
    DV_CORE_WARN(message);
    return;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    DV_CORE_INFO(message);
    return;
  default:
    DV_CORE_ASSERT(false, "Unknown severity level");
    return;
  }
}

void OpenGLRendererAPI::Init() {
  DV_PROFILE_FUNCTION();

#ifdef DV_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                        GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height) {
  glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray,
                                    uint32_t indexCount) {
  uint32_t count =
      indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Develle
