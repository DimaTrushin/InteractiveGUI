#pragma once

#include "ApplicationGUI.h"
#include "ApplicationKernel.h"
#include "Kernel/FieldController.h"

namespace QApp {

class ApplicationImpl : protected ApplicationKernel, protected ApplicationGUI {
  using PlotController = Interface::PlotController;
  using FieldController = Kernel::FieldController;

public:
  ApplicationImpl();

private:
  PlotController controller_;
  FieldController field_controller_;
};

} // namespace QApp
