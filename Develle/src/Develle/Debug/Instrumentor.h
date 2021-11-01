#ifndef INSTRUMENTOR_H_
#define INSTRUMENTOR_H_

#include <Tracy.hpp>

#if DV_PROFILE

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

#else // DV_PROFILE

#define DV_PROFILE_SCOPE(name)
#define DV_PROFILE_FUNCTION()

#endif // DV_PROFILE
#endif // INSTRUMENTOR_H_
