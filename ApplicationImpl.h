#pragma once

#include "Interface/MainWindow.h"

#include "Interface/View.h"
#include "Kernel/GeomModel.h"

namespace QApp {

class ApplicationImpl {
  using MainWindow = Interface::MainWindow;
  using GeomModel = Kernel::GeomModel;
  using View = Interface::View;

public:
  ApplicationImpl();

private:
  MainWindow main_window_;
  GeomModel model_;
  View view_;
};

} // namespace QApp
