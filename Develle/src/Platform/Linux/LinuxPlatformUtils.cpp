#ifndef LINUXPLATFORMUTILS_HPP_
#define LINUXPLATFORMUTILS_HPP_

#include <portable-file-dialogs.h>

#include <Develle/Core/Application.hpp>
#include <Develle/Utils/PlatformUtils.hpp>

namespace Develle {

auto FileDialogs::OpenFolder(const std::string& dialogTitle) -> std::string {
  auto f = pfd::select_folder(dialogTitle, "${HOME}").result();
  return f;
}

auto FileDialogs::OpenFile(const std::vector<std::string>& filters) -> std::string {
  auto f = pfd::open_file("Open File", "${HOME}", filters, pfd::opt::none).result();
  if (f.empty()) return {};

  return f[0];
}

auto FileDialogs::SaveFile(const std::vector<std::string>& filters) -> std::string {
  auto f = pfd::save_file("Save File", "", filters, pfd::opt::none).result();
  if (f.empty()) return {};

  return f;
}

}  // namespace Develle
#endif  // LINUXPLATFORMUTILS_HPP_
