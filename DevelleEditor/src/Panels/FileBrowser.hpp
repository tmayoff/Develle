#pragma once
#ifndef FILEBROWSER_HPP_
#define FILEBROWSER_HPP_

#include <filesystem>

namespace Develle {

class FileBrowser {
 public:
  FileBrowser() = default;
  FileBrowser(const std::string& path);

  void OnImGuiRender();

 private:
  std::filesystem::path projectPath;
};

}  // namespace Develle
#endif  // FILEBROWSER_HPP_
