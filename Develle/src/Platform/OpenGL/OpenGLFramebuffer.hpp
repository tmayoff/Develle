#ifndef OPENGLFRAMEBUFFER_HPP_
#define OPENGLFRAMEBUFFER_HPP_

#include <Develle/Renderer/Framebuffer.hpp>

namespace Develle {

class OpenGLFramebuffer : public Framebuffer {
public:
  OpenGLFramebuffer(const FramebufferSpecification &spec);
  ~OpenGLFramebuffer();

  void Invalidate();

  void Bind() override;
  void Unbind() override;

  void Resize(uint32_t width, uint32_t height) override;
  int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

  void ClearAttachment(uint32_t attachmentIndex, int value) override;

  uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override {
    DV_CORE_ASSERT(index < colorAttachments.size(),
                   "Color attachment index out of range");
    return colorAttachments[index];
  }

  const FramebufferSpecification &GetSpecification() const override {
    return specification;
  }

private:
  uint32_t rendererID = 0;
  FramebufferSpecification specification;

  std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
  FramebufferTextureSpecification depthAttachmentSpecification =
      FramebufferTextureFormat::None;

  std::vector<uint32_t> colorAttachments;
  uint32_t depthAttachment = 0;
};

} // namespace Develle
#endif // OPENGLFRAMEBUFFER_HPP_
