#ifndef LOG_H_
#define LOG_H_

#define FMT_HEADER_ONLY
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include <Develle/Core/Core.h>

namespace Develle {

class Log {
public:
  static void Init();

  static Ref<spdlog::logger> &GetLogger() { return coreLogger; }
  static Ref<spdlog::logger> &GetClientLogger() { return clientLogger; }

private:
  static Ref<spdlog::logger> coreLogger;
  static Ref<spdlog::logger> clientLogger;
};

} // namespace Develle

// Clore log macros
#define DV_CORE_TRACE(...) ::Develle::Log::GetLogger()->trace(__VA_ARGS__)
#define DV_CORE_INFO(...) ::Develle::Log::GetLogger()->info(__VA_ARGS__)
#define DV_CORE_WARN(...) ::Develle::Log::GetLogger()->warn(__VA_ARGS__)
#define DV_CORE_ERROR(...) ::Develle::Log::GetLogger()->error(__VA_ARGS__)
#define DV_CORE_FATAL(...) ::Develle::Log::GetLogger()->fatal(__VA_ARGS__)

// Client log macros
#define DV_CLIENT_TRACE(...)                                                   \
  ::Develle::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DV_CLIENT_INFO(...) ::Develle::Log::GetClientLogger()->info(__VA_ARGS__)
#define DV_CLIENT_WARN(...) ::Develle::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DV_CLIENT_ERROR(...)                                                   \
  ::Develle::Log::GetClientLogger()->error(__VA_ARGS__)
#define DV_CLIENT_FATAL(...)                                                   \
  ::Develle::Log::GetClientLogger()->fatal(__VA_ARGS__)

#endif // LOG_H_
