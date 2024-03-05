#pragma once

#include "Interface/MainWindow.h"
#include "Interface/PlotController.h"
#include "Interface/View.h"

namespace QApp {

class ApplicationGUI {
  using MainWindow = Interface::MainWindow;
  using View = Interface::View;

public:
  ApplicationGUI();

protected:
  MainWindow main_window_;
  View view_;
};

} // namespace QApp
