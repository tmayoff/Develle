#include "FileBrowser.hpp"

#include <imgui.h>

namespace Develle {

FileBrowser::FileBrowser(const std::string& path) : projectPath(path) {}

void FileBrowser::OnImGuiRender() {
  ImGui::Begin("File Browser");

  ImGui::End();
}

}  // namespace Develle
