#ifndef APPLICATIONEVENT_HPP_
#define APPLICATIONEVENT_HPP_

#include <Develle/Events/Event.hpp>

namespace Develle {

class WindowResizeEvent : public Event {
public:
  WindowResizeEvent(uint32_t width, uint32_t height)
      : width(width), height(height) {}

  uint32_t GetWidth() const { return width; }
  uint32_t GetHeight() const { return height; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent" << width << ", " << height;
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
  uint32_t width, height;
};

class WindowCloseEvent : public Event {
public:
  WindowCloseEvent() = default;

  EVENT_CLASS_TYPE(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

} // namespace Develle

#endif // APPLICATIONEVENT_HPP_
