#ifndef UNIFORMBUFFER_HPP_
#define UNIFORMBUFFER_HPP_

#include <Develle/Core/Core.h>

namespace Develle {

class UniformBuffer {
public:
  virtual ~UniformBuffer() {}

  virtual void SetData(const void *data, uint32_t size,
                       uint32_t offset = 0) = 0;

  static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
};

} // namespace Develle
#endif // UNIFORMBUFFER_HPP_
