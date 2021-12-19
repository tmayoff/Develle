#ifndef ENTRYPOINT_H_
#define ENTRYPOINT_H_

#include "Application.hpp"
#include "Core.hpp"

extern Develle::Application *Develle::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char **argv) {
  Develle::Log::Init();

  auto app = Develle::CreateApplication({argc, argv});

  app->Run();

  delete app;
}

#endif  // ENTRYPOINT_H_
