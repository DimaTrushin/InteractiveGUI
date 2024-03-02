#pragma once

#include "Kernel/MouseAction.h"
#include "Library/Observer3/Observer.h"

#include <optional>

namespace QApp {
namespace Kernel {
class GeomModel;
}

namespace Interface {

class PlotController {
  using GeomModel = Kernel::GeomModel;
  using EMouseStatus = Kernel::EMouseStatus;
  using MouseAction = Kernel::MouseAction;
  using MouseData = std::optional<MouseAction>;
  using Observer = NSLibrary::CObserver<MouseData>;
  using MouseDataArg = Observer::CArg;

public:
  PlotController(GeomModel* host);

  Observer* port();

private:
  void control(MouseDataArg data);
  void controlOnData(const MouseAction& action);

  GeomModel* host_;
  Observer port_;
};

} // namespace Interface
} // namespace QApp
