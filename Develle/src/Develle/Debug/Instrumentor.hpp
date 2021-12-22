#ifndef INSTRUMENTOR_HPP_
#define INSTRUMENTOR_HPP_

#include <Tracy.hpp>

#ifdef DV_PROFILE

// void *operator new(std::size_t count) {
//   auto ptr = malloc(count);
//   TracyAlloc(ptr, count);
//   return ptr;
// }

// void operator delete(void *ptr) noexcept {
//   TracyFree(ptr);
//   free(ptr);
// }

#define DV_PROFILE_SCOPE(name) ZoneScopedN(name)
#define DV_PROFILE_FUNCTION() ZoneScoped
#define DV_PROFILE_THREAD(name) tracy::SetThreadName(name)
#define DV_PROFILE_FRAME(name) FrameMarkNamed(name)

#else  // DV_PROFILE

#define DV_PROFILE_SCOPE(name)
#define DV_PROFILE_FUNCTION()
#define DV_PROFILE_THREAD(name)
#define DV_PROFILE_THREAD(name)

#endif  // DV_PROFILE

#endif  // INSTRUMENTOR_HPP_
