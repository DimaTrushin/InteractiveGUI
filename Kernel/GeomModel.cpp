#include "GeomModel.h"

#include <cassert>

namespace QApp {
namespace Kernel {

GeomModel::GeomModel() {
  //  QObject::connect(&timer_, &QTimer::timeout, this, &GeomModel::onTimer);
  //  timer_.start(timeout_);
}

void GeomModel::subscribe(Observer* obs) {
  assert(obs);
  port_.subscribe(obs);
}

void GeomModel::onTimer() {
  static int dx = 0;
  static int dy = 0;
  data_->item1.center = QPointF(dx % 10, dy % 10);
  data_->item2.center = QPointF(dx % 5, dy % 7);
  ++dx;
  ++dy;
  port_.notify();
}

} // namespace Kernel
} // namespace QApp
