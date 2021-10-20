#ifndef CORE_H_
#define CORE_H_

#include <memory>

#ifdef DV_DEBUG
#if defined(DV_PLATFORM_WINDOWS)
#define DV_DEBUGBREAK() __debugbreak()
#elif defined(DV_PLATFORM_LINUX)
#include <signal.h>
#define DV_DEBUGBREAK raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#else
#define DV_DEBUGBREAK()
#endif

#define DV_EXPAND_MACRO(x) x
#define DV_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define DV_BIND_EVENT_FN(fn)                                                   \
  [this](auto &&...args) -> decltype(auto) {                                   \
    return this->fn(std::forward<decltype(args)>(args)...);                    \
  }

namespace Develle {

template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace Develle

#include <Develle/Core/Assert.h>
#include <Develle/Core/Log.h>

#endif // CORE_H_
