#include "GeomModel.h"

#include <QDebug>

#include <cassert>
#include <ranges>

namespace QApp {
namespace Kernel {

GeomModel::GeomModel()
    : in_port_([this](FieldDataArg data) { onFieldData(data); },
               [this](FieldDataArg data) { onFieldData(data); },
               ObserverField::doNothing) {
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

GeomModel::ObserverField* GeomModel::port() {
  return &in_port_;
}

void GeomModel::onMousePress(const QPointF& position) {
  int index = touchedItem(position);
  if (index != k_non) {
    active_index_ = index;
    assert(data_.has_value());
    diff_ = position - data_->items[index].center;
  }
}

void GeomModel::onMouseMove(const QPointF& position) {
  if (data_.has_value() && active_index_ != k_non) {
    data_->items[active_index_].center = position - diff_;
    port_.notify();
  }
}

void GeomModel::onMouseRelease(const QPointF&) {
  active_index_ = k_non;
  diff_ = {0., 0.};
}

int GeomModel::touchedItem(const QPointF& position) const {
  if (!data_.has_value())
    return k_non;
  int index = data_->items.size() - 1;
  for (const auto& item : std::ranges::reverse_view(data_->items)) {
    QPointF diff = item.center - position;
    if (std::sqrt(QPointF::dotProduct(diff, diff)) < item.radius)
      return index;
    --index;
  }
  return k_non;
}

void GeomModel::onFieldData(FieldDataArg data) {
  if (!data.has_value()) {
    if (data_.has_value()) {
      data_.reset();
      port_.notify();
    }
    return;
  }
  const Field& field = *data;
  if (!data_.has_value())
    data_.emplace();
  data_->field.rows = field.rows();
  data_->field.columns = field.columns();
  data_->field.hight = k_hight;
  data_->field.width = k_width;
  data_->items.reserve(field.items().size());
  for (const auto& item : field.items()) {
    using DrawItem = DrawData::Item;
    data_->items.emplace_back(
        DrawItem{.center = {0.5 + item.row(), 0.5 + item.column()}});
  }
  port_.notify();
}

} // namespace Kernel
} // namespace QApp
