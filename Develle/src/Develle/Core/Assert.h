#ifndef ASSERT_H_
#define ASSERT_H_

#include <Develle/Core/Core.h>
// #include <Develle/Core/Log.h>

#include <filesystem>

#ifdef DV_ENABLE_ASSERTS

#define DV_INTERNAL_ASSERT_IMPL(type, check, msg, ...)                         \
  {                                                                            \
    if (!(check)) {                                                            \
      DV##type##ERROR(msg, __VA_ARGS__);                                       \
      DV_DEBUGBREAK;                                                           \
    }                                                                          \
  }

#define DV_INTERNAL_ASSERT_WITH_MSG(type, check, ...)                          \
  DV_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)

#define DV_INTERNAL_ASSERT_NO_MSG(type, check)                                 \
  DV_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}', failed at {1}:{2}",   \
                          DV_STRINGIFY_MACRO(check),                           \
                          std::filesystem::path(__FILE__).filename().string(), \
                          __LINE__)

#define DV_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define DV_INTERNAL_ASSERT_GET_MACRO(...)                                      \
  DV_EXPAND_MACRO(DV_INTERNAL_ASSERT_GET_MACRO_NAME(                           \
      __VA_ARGS__, DV_INTERNAL_ASSERT_WITH_MSG, DV_INTERNAL_ASSERT_NO_MSG))

#define DV_ASSERT(...)                                                         \
  DV_EXPAND_MACRO(DV_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define DV_CORE_ASSERT(...)                                                    \
  DV_EXPAND_MACRO(                                                             \
      DV_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))

#else

#define DV_ASSERT(...)
#define DV_CORE_ASSERT(...)

#endif //

#endif // ASSERT_H_
