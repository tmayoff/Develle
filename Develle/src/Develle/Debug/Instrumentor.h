#ifndef INSTRUMENTOR_H_
#define INSTRUMENTOR_H_

#include <Tracy.hpp>

namespace Develle {

#define DV_PROFILE 1
#if DV_PROFILE

#define DV_PROFILE_SCOPE(name) ZoneScopedN(name)
#define DV_PROFILE_FUNCTION() ZoneScoped

#else

#define DV_PROFILE_BEGIN_SESSION(name, filepath)
#define DV_PROFILE_END_SESSION()
#define DV_PROFILE_SCOPE_LINE(name, line)
#define DV_PROFILE_SCOPE(name)
#define DV_PROFILE_FUNCTION()

#endif

} // namespace Develle
#endif
