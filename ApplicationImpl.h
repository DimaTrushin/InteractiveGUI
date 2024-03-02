#pragma once

#include "Interface/MainWindow.h"

#include "Interface/PlotController.h"
#include "Interface/View.h"
#include "Kernel/GeomModel.h"

namespace QApp {

class ApplicationImpl {
  using MainWindow = Interface::MainWindow;
  using GeomModel = Kernel::GeomModel;
  using View = Interface::View;
  using PlotController = Interface::PlotController;

public:
  ApplicationImpl();

private:
  MainWindow main_window_;
  GeomModel geom_model_;
  View view_;
  PlotController controller_;
};

} // namespace QApp
