#ifndef PLATFORMUTILS_HPP_
#define PLATFORMUTILS_HPP_

namespace Develle {

class FileDialogs {
 public:
  static std::string OpenFile(const std::vector<std::string>& filter);
  static std::string SaveFile(const std::vector<std::string>& filter);
};

}  // namespace Develle
#endif  // PLATFORMUTILS_HPP_
