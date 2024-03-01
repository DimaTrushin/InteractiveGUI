#pragma once

#include "Interface/MainWindow.h"

namespace QApp {

class ApplicationImpl {
  using MainWindow = Interface::MainWindow;

public:
  ApplicationImpl();

private:
  MainWindow main_window_;
};

} // namespace QApp
