#ifndef ENTRYPOINT_H_
#define ENTRYPOINT_H_

#include <Develle/Core/Application.h>
#include <Develle/Core/Core.h>

extern Develle::Application *
Develle::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char **argv) {
  Develle::Log::Init();

  auto app = Develle::CreateApplication({argc, argv});

  app->Run();

  delete app;
}

#endif // ENTRYPOINT_H_
