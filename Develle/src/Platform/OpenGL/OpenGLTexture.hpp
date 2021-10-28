#ifndef OPENGLTEXTURE_HPP_
#define OPENGLTEXTURE_HPP_

#include <Develle/Renderer/Texture.hpp>

#include <glad/glad.h>

namespace Develle {

class OpenGLTexture2D : public Texture2D {
public:
  OpenGLTexture2D(uint32_t width, uint32_t height);
  OpenGLTexture2D(const std::string &path);
  ~OpenGLTexture2D();

  uint32_t GetWidth() const override { return width; }
  uint32_t GetHeight() const override { return height; }
  uint32_t GetRendererID() const override { return rendererID; }

  void SetData(void *data, uint32_t size) override;

  void Bind(uint32_t slot = 0) const override;

  bool IsLoaded() const override { return isLoaded; }

  bool operator==(const Texture &other) const override {
    return rendererID == ((OpenGLTexture2D &)other).rendererID;
  }

private:
  std::string path;
  bool isLoaded = false;
  uint32_t width, height;
  uint32_t rendererID;
  GLenum internalFormat, dataFormat;
};

} // namespace Develle
#endif // OPENGLTEXTURE_HPP_
