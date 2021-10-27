#ifndef KEYEVENT_HPP_
#define KEYEVENT_HPP_

#include <Develle/Core/KeyCodes.hpp>
#include <Develle/Events/Event.hpp>

namespace Develle {

class KeyEvent : public Event {
public:
  KeyCode GetKeyCode() const { return keycode; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
  KeyEvent(const KeyCode keycode) : keycode(keycode) {}

  KeyCode keycode;
};

class KeyPressedEvent : public KeyEvent {
public:
  KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
      : KeyEvent(keycode), repeatCount(repeatCount) {}

  uint16_t GetRepeatCount() const { return repeatCount; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << keycode << " (" << repeatCount << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)

private:
  uint16_t repeatCount;
};

class KeyReleasedEvent : public KeyEvent {
public:
  KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << keycode;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public Event {
public:
  KeyTypedEvent(const char *key) : c(key) {}

  const char *GetKey() const { return c; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyTypedEvent: " << c;
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyTyped)
  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

private:
  const char *c;
};

} // namespace Develle
#endif // KEYEVENT_HPP_
