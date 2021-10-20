#ifndef ENTRYPOINT_H_
#define ENTRYPOINT_H_

#include <Develle/Core/Application.h>
#include <Develle/Core/Core.h>

extern Develle::Application *
Develle::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char **argv) {
  Develle::Log::Init();

  DV_PROFILE_BEGIN_SESSION("Startup", "DevelleProfile-Startup.json");
  auto app = Develle::CreateApplication({argc, argv});
  DV_PROFILE_END_SESSION();

  DV_PROFILE_BEGIN_SESSION("Runtime", "DevelleProfile-Runtime.json");
  app->Run();
  DV_PROFILE_END_SESSION();

  DV_PROFILE_BEGIN_SESSION("Shutdown", "DevelleProfile-Shutdown.json");
  delete app;
  DV_PROFILE_END_SESSION();
}

#endif // ENTRYPOINT_H_
