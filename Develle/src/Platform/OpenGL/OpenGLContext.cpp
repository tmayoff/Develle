#include "OpenGLContext.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

namespace Develle {

OpenGLGraphicsContext::OpenGLGraphicsContext(SDL_Window *window)
    : windowHandle(window) {
  DV_CORE_ASSERT(windowHandle, "Window handle is null");
}

void OpenGLGraphicsContext::Init() {
  DV_PROFILE_FUNCTION();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  context = SDL_GL_CreateContext(windowHandle);
  DV_CORE_ASSERT(context, SDL_GetError());

  SDL_GL_MakeCurrent(windowHandle, context);

  int status = gladLoadGL();
  DV_CORE_ASSERT(gladLoadGL(); == GL_TRUE, "Failed to initialize GLAD");

  DV_CORE_INFO("OpenGL Info");
  DV_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
  DV_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
  DV_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

  DV_CORE_ASSERT(GLVersion.major > 4 ||
                     (GLVersion.major == 4 && GLVersion.minor >= 5),
                 "Develle requires at least OpenGL version 4.5");
}

void OpenGLGraphicsContext::SwapBuffers() {
  DV_PROFILE_FUNCTION();
  SDL_GL_SwapWindow(windowHandle);
}
} // namespace Develle
