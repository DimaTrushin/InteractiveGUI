#pragma once

#include "Interface/MainWindow.h"
#include "Interface/PlotController.h"
#include "Interface/View.h"

namespace QApp {

class ApplicationGUI {
  using MainWindow = Interface::MainWindow;
  using View = Interface::View;

public:
protected:
  MainWindow* mainWindow();
  View* view();
  View* viewg();
  View* view2();

private:
  MainWindow main_window_;
};

} // namespace QApp
