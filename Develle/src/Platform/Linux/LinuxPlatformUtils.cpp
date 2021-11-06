#ifndef LINUXPLATFORMUTILS_HPP_
#define LINUXPLATFORMUTILS_HPP_
#include <Develle/Utils/PlatformUtils.hpp>

//
#include <Develle/Core/Application.h>
#include <portable-file-dialogs.h>

namespace Develle {

std::string FileDialogs::OpenFile(const std::vector<std::string>& filters) {
  auto f = pfd::open_file("Open File", "${HOME}", filters, false).result();
  if (f.empty()) return std::string();

  return f[0];
}

std::string FileDialogs::SaveFile(const std::vector<std::string>& filters) {
  auto f = pfd::save_file("Save File", "", filters, true).result();
  if (f.empty()) return std::string();

  return f;
}

}  // namespace Develle
#endif  // LINUXPLATFORMUTILS_HPP_
