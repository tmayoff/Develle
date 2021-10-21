#ifndef COMPONENTS_H_
#define COMPONENTS_H_

namespace Develle {

struct TagComponent {
  std::string Tag;

  TagComponent() = default;
  TagComponent(const TagComponent &) = default;
  TagComponent(const std::string &tag) : Tag(tag) {}
};

} // namespace Develle
#endif // COMPONENTS_H_
