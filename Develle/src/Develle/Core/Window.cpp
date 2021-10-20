#include <Develle/Core/Window.h>

#ifdef DV_PLATFORM_LINUX
#include <Platform/Linux/LinuxWindow.h>
#endif

namespace Develle {

Scope<Window> Window::Create(const WindowProps &props) {
#ifdef DV_PLATFORM_LINUX
  return CreateScope<LinuxWindow>(props);
#else
  DV_CORE_ASSERT(false, "Unkown Platform!");
  return nullptr;
#endif
}

} // namespace Develle
