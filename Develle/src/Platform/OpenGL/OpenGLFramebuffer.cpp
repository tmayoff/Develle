#include "OpenGLFramebuffer.hpp"

#include <glad/glad.h>

namespace Develle {

static const uint32_t MaxFramebufferSize = 8192;

namespace Utils {

static GLenum TextureTarget(bool multisampled) {
  return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
  glCreateTextures(TextureTarget(multisampled), count, outID);
}

static void BindTexture(bool multisampled, uint32_t id) {
  glBindTexture(TextureTarget(multisampled), id);
}

static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format,
                               uint32_t width, uint32_t height, int index) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height,
                            GL_FALSE);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE,
                 nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled),
                         id, 0);
}

static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType,
                               uint32_t width, uint32_t height) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
  } else {
    glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
}

static bool IsDepthFormat(FramebufferTextureFormat format) {
  switch (format) {
    case FramebufferTextureFormat::DEPTH24STENCIL8:
      return true;
    default:
      return false;
  }
}

static GLenum DevelleFBTextureFormatToGL(FramebufferTextureFormat format) {
  switch (format) {
    case FramebufferTextureFormat::RGBA8:
      return GL_RGBA8;
    case FramebufferTextureFormat::RED_INTEGER:
      return GL_RED_INTEGER;
    default:
      DV_CORE_ASSERT(false, "Unknown Framebuffer format");
      return 0;
  }
}

}  // namespace Utils

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : specification(spec) {
  for (auto spec : specification.Attachments.Attachments) {
    if (!Utils::IsDepthFormat(spec.TextureFormat))
      colorAttachmentSpecifications.emplace_back(spec);
    else
      depthAttachmentSpecification = spec;
  }

  Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
  glDeleteFramebuffers(1, &rendererID);
  glDeleteTextures(colorAttachments.size(), colorAttachments.data());
  glDeleteTextures(1, &depthAttachment);
}

void OpenGLFramebuffer::Invalidate() {
  if (rendererID) {
    glDeleteFramebuffers(1, &rendererID);
    glDeleteTextures(colorAttachments.size(), colorAttachments.data());
    glDeleteTextures(1, &depthAttachment);

    colorAttachments.clear();
    depthAttachment = 0;
  }

  glCreateFramebuffers(1, &rendererID);
  glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

  bool multisample = specification.Samples > 1;

  // Attachments
  if (colorAttachmentSpecifications.size()) {
    colorAttachments.resize(colorAttachmentSpecifications.size());
    Utils::CreateTextures(multisample, colorAttachments.data(), colorAttachments.size());

    for (size_t i = 0; i < colorAttachments.size(); i++) {
      Utils::BindTexture(multisample, colorAttachments[i]);
      switch (colorAttachmentSpecifications[i].TextureFormat) {
        case FramebufferTextureFormat::RGBA8:
          Utils::AttachColorTexture(colorAttachments[i], specification.Samples, GL_RGBA8, GL_RGBA,
                                    specification.Width, specification.Height, i);
          break;
        case FramebufferTextureFormat::RED_INTEGER:
          Utils::AttachColorTexture(colorAttachments[i], specification.Samples, GL_R32I,
                                    GL_RED_INTEGER, specification.Width, specification.Height, i);
          break;
      }
    }
  }

  if (depthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
    Utils::CreateTextures(multisample, &depthAttachment, 1);
    Utils::BindTexture(multisample, depthAttachment);
    switch (depthAttachmentSpecification.TextureFormat) {
      case FramebufferTextureFormat::DEPTH24STENCIL8:
        Utils::AttachDepthTexture(depthAttachment, specification.Samples, GL_DEPTH24_STENCIL8,
                                  GL_DEPTH_STENCIL_ATTACHMENT, specification.Width,
                                  specification.Height);
        break;
    }
  }

  if (colorAttachments.size() > 1) {
    DV_CORE_ASSERT(colorAttachments.size() <= 4, "We don't support more than 4 color attachments");
    // std::array<GLenum, 4> buffers = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
    //                                  GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    GLenum buffers[4]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(colorAttachments.size(), buffers);
  } else if (colorAttachments.empty()) {
    glDrawBuffer(GL_NONE);
  }

  DV_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                 "Framebuffer is incomplete");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
  glViewport(0, 0, specification.Width, specification.Height);
}

void OpenGLFramebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
  if (width == 0 || height == 0 || width > MaxFramebufferSize || height > MaxFramebufferSize) {
    DV_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
    return;
  }

  specification.Width = width;
  specification.Height = height;

  Invalidate();
}

int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
  DV_CORE_ASSERT(attachmentIndex < colorAttachments.size(), "Color attachment index out of bounds");

  glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
  int pixelData;
  glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
  return pixelData;
}

void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
  DV_CORE_ASSERT(attachmentIndex < colorAttachments.size(), "Color attachment index out of bounds");

  auto& spec = colorAttachmentSpecifications[attachmentIndex];
  glClearTexImage(colorAttachments[attachmentIndex], 0,
                  Utils::DevelleFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
}

}  // namespace Develle
