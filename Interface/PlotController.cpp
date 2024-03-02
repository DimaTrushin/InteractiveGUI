#include "PlotController.h"

#include "Kernel/GeomModel.h"

#include <QDebug>

#include <cassert>

namespace QApp {
namespace Interface {

PlotController::PlotController(GeomModel* host)
    : host_(host),
      port_([this](MouseDataArg data) { control(data); },
            [this](MouseDataArg data) { control(data); }, Observer::doNothing) {
  assert(host_);
}

PlotController::Observer* PlotController::port() {
  return &port_;
}

void PlotController::control(MouseDataArg data) {
  if (data.has_value())
    controlOnData(*data);
}

void PlotController::controlOnData(const MouseAction& action) {
  host_->onMouseAction(action);
}

} // namespace Interface
} // namespace QApp
