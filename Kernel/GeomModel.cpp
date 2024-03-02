#include "GeomModel.h"

#include <QDebug>

#include <cassert>

namespace QApp {
namespace Kernel {

GeomModel::GeomModel() {
}

void GeomModel::subscribe(Observer* obs) {
  assert(obs);
  port_.subscribe(obs);
}

void GeomModel::onMouseAction(const MouseAction& action) {
  switch (action.status) {
  case EMouseStatus::Pressed:
    onMousePress(action.position);
    break;
  case EMouseStatus::Moved:
    onMouseMove(action.position);
    break;
  case EMouseStatus::Released:
    onMouseRelease(action.position);
    break;
  default:
    assert(false);
    break;
  }
}

void GeomModel::onMousePress(const QPointF& position) {
  if (isItem1Touched(position)) {
    status_ = ItemStatus::Active;
    diff_ = position - data_->item1.center;
  }
}

void GeomModel::onMouseMove(const QPointF& position) {
  if (status_ == ItemStatus::Active && data_.has_value()) {
    data_->item1.center = position - diff_;
    port_.notify();
  }
}

void GeomModel::onMouseRelease(const QPointF&) {
  status_ = ItemStatus::Inactive;
  diff_ = {0., 0.};
}

bool GeomModel::isItem1Touched(const QPointF& position) const {
  if (!data_.has_value())
    return false;
  QPointF diff = data_->item1.center - position;
  return std::sqrt(QPointF::dotProduct(diff, diff)) < data_->item1.radius;
}

} // namespace Kernel
} // namespace QApp
