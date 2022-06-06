#include "FileBrowser.hpp"

#include <imgui.h>

#include <filesystem>

namespace Develle {

FileBrowser::FileBrowser() : projectPath(ASSETS_ROOT), currentDirectory(projectPath) {
  directoryIcon = Texture2D::Create("../icons/filebrowser/folder.png");
  fileIcon = Texture2D::Create("icons/filebrowser/file.png");
}

FileBrowser::FileBrowser(const std::string& path)
    : projectPath(path), currentDirectory(projectPath) {
  directoryIcon = Texture2D::Create("icons/filebrowser/folder.png");
  fileIcon = Texture2D::Create("icons/filebrowser/file.png");
}

void FileBrowser::SetProjectPath(const std::string& path) {
  projectPath = path;
  currentDirectory = projectPath;
}

void FileBrowser::OnImGuiRender() {
  ImGui::Begin("File Browser");

  if (currentDirectory != std::filesystem::path(projectPath)) {
    if (ImGui::Button("<-")) currentDirectory = currentDirectory.parent_path();
  }

  static float padding = 16.0F;
  static float thumbnailSize = 128.0F;
  float cellSize = thumbnailSize + padding;

  float panelWidth = ImGui::GetContentRegionAvail().x;
  int columntCount = static_cast<int>(panelWidth / cellSize);
  if (columntCount < 1) columntCount = 1;

  ImGui::Columns(columntCount, nullptr, false);

  if (std::filesystem::exists(currentDirectory)) {
    for (const auto& directoryEntry : std::filesystem::directory_iterator(currentDirectory)) {
      const auto& path = directoryEntry.path();
      auto relativePath = std::filesystem::relative(path, projectPath);
      std::string filename = relativePath.filename().string();

      ImGui::PushID(filename.c_str());
      Ref<Texture2D> icon = directoryEntry.is_directory() ? directoryIcon : fileIcon;
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
      ImGui::ImageButton((ImTextureID)icon->GetRendererID(), {thumbnailSize, thumbnailSize}, {0, 1},
                         {1, 0});

      if (ImGui::BeginDragDropSource()) {
        const auto* itemPath = relativePath.c_str();
        ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath,
                                  (strlen(itemPath) + 1) * sizeof(char));
        ImGui::EndDragDropSource();
      }

      ImGui::PopStyleColor();

      if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        if (directoryEntry.is_directory()) currentDirectory /= path.filename();
      }
      ImGui::TextWrapped("%s", filename.c_str());

      ImGui::NextColumn();

      ImGui::PopID();
    }
  }

  ImGui::Columns(1);

  ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
  ImGui::SliderFloat("Padding", &padding, 0, 32);

  ImGui::End();
}

}  // namespace Develle
