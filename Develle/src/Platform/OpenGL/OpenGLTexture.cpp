#include <Platform/OpenGL/OpenGLTexture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace Develle {

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : width(width), height(height) {
  DV_PROFILE_FUNCTION();

  internalFormat = GL_RGBA8;
  dataFormat = GL_RGBA;

  glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
  glTextureStorage2D(rendererID, 1, internalFormat, width, height);

  glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string &path) : path(path) {
  DV_PROFILE_FUNCTION();

  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc *data = nullptr;
  {
    DV_PROFILE_SCOPE(
        "stbi_load - OpenGLTexture2D::OpenGLTexture2D(const "
        "std::string& name)");
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  }

  if (data) {
    isLoaded = true;

    this->width = width;
    this->height = height;

    internalFormat = 0, dataFormat = 0;
    if (channels == 4) {
      internalFormat = GL_RGBA8;
      dataFormat = GL_RGBA;
    } else if (channels == 3) {
      internalFormat = GL_RGB8;
      dataFormat = GL_RGB;
    }

    DV_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

    glCreateTextures(GL_TEXTURE_2D, 1, &rendererID);
    glTextureStorage2D(rendererID, 1, internalFormat, width, height);

    glTextureParameteri(rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  } else {
    DV_CORE_ERROR("Failed to load Texture");
  }
}

OpenGLTexture2D::~OpenGLTexture2D() {
  DV_PROFILE_FUNCTION();

  glDeleteTextures(1, &rendererID);
}

void OpenGLTexture2D::SetData(void *data, uint32_t size) {
  DV_PROFILE_FUNCTION();

  uint32_t bpp = dataFormat == GL_RGBA ? 4 : 3;
  DV_CORE_ASSERT(size == width * height * bpp, "Data must be entire texture");
  glTextureSubImage2D(rendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
  DV_PROFILE_FUNCTION();

  glBindTextureUnit(slot, rendererID);
}

}  // namespace Develle
