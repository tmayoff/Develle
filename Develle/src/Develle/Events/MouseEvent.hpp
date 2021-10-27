#ifndef MOUSEEVENT_HPP_
#define MOUSEEVENT_HPP_

#include <Develle/Core/MouseCodes.hpp>
#include <Develle/Events/Event.hpp>

namespace Develle {

class MouseMovedEvent : public Event {
public:
  MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}

  float GetX() const { return mouseX; }
  float GetY() const { return mouseY; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseMoved)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
  float mouseX, mouseY;
};

class MouseScrolledEvent : public Event {
public:
  MouseScrolledEvent(const float xOffset, const float yOffset)
      : xOffset(xOffset), yOffset(yOffset) {}

  float GetXOffset() const { return xOffset; }
  float GetYOffset() const { return yOffset; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseScrolled)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
  float xOffset, yOffset;
};

class MouseButtonEvent : public Event {
public:
  MouseCode GetMouseButton() const { return button; }

  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput |
                       EventCategoryMouseButton)

protected:
  MouseButtonEvent(const MouseCode button) : button(button) {}

  MouseCode button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
  MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << button;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
  MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << button;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace Develle
#endif // MOUSEEVENT_HPP_
