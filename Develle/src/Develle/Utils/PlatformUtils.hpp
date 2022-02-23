#ifndef PLATFORMUTILS_HPP_
#define PLATFORMUTILS_HPP_

#include <string>
#include <vector>

namespace Develle {

class FileDialogs {
 public:
  static std::string OpenFolder(const std::string& dialogTitle);
  static std::string OpenFile(const std::vector<std::string>& filter);
  static std::string SaveFile(const std::vector<std::string>& filter);
};

}  // namespace Develle
#endif  // PLATFORMUTILS_HPP_
