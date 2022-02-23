#pragma once
#ifndef FILEBROWSER_HPP_
#define FILEBROWSER_HPP_

#include <Develle/Renderer/Texture.hpp>
#include <filesystem>

namespace Develle {

class FileBrowser {
 public:
  FileBrowser();
  FileBrowser(const std::string& path);

  void SetProjectPath(const std::string& path);

  void OnImGuiRender();

 private:
  Ref<Texture2D> directoryIcon;
  Ref<Texture2D> fileIcon;

  std::filesystem::path projectPath;

  std::filesystem::path currentDirectory;
};

}  // namespace Develle
#endif  // FILEBROWSER_HPP_
