#include "GeomModel.h"

#include <QDebug>

#include <cassert>
#include <ranges>

namespace QApp {
namespace Kernel {

GeomModel::GeomModel()
    : in_port_([this](FieldData&& data) { onFieldData(std::move(data)); }) {
}

void GeomModel::subscribeToDrawData(Observer* obs) {
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

void GeomModel::subscribeToItemAction(ObserverAction* obs) {
  assert(obs);
  action_port_.subscribe(obs);
}

void GeomModel::onMousePress(const QPointF& position) {
  int index = touchedItem(position);
  if (index != k_non) {
    active_index_ = index;
    assert(data_.has_value());
    diff_ = position - data_->items[active_index_].center;
    data_->items[active_index_].fill = palette_.fill(ItemStatus::Active);
    data_->items[active_index_].countur = palette_.countur(ItemStatus::Active);
    port_.notify();
  }
}

void GeomModel::onMouseMove(const QPointF& position) {
  if (data_.has_value() && active_index_ != k_non) {
    data_->items[active_index_].center = position - diff_;
    port_.notify();
  }
}

void GeomModel::onMouseRelease(const QPointF& position) {
  if (active_index_ == k_non)
    return;
  assert(data_.has_value());
  diff_ = {0., 0.};
  size_t index = std::exchange(active_index_, k_non);
  int row = getRow(position);
  int column = getColumn(position);
  action_port_.set(std::in_place_t(), index, row, column);
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

int GeomModel::getRow(const QPointF& position) const {
  assert(data_.has_value());
  const DrawData::FieldData& field = data_->field;
  return std::floor((position.y() - field.origin.y()) / field.hight);
}

int GeomModel::getColumn(const QPointF& position) const {
  const DrawData::FieldData& field = data_->field;
  return std::floor((position.x() - field.origin.x()) / field.width);
}

void GeomModel::onFieldData(FieldData&& data) {
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
  data_->field.hight = palette_.field_hight;
  data_->field.width = palette_.field_width;
  data_->field.origin = palette_.field_origin;

  data_->items.clear();
  data_->items.reserve(field.items().size());
  for (const auto& item : field.items()) {
    using DrawItem = DrawData::Item;
    data_->items.emplace_back(
        DrawItem{.center = QPointF{(0.5 + item.column()) * data_->field.width,
                                   (0.5 + item.row()) * data_->field.hight} +
                           data_->field.origin,
                 .radius = palette_.item_radius,
                 .fill = palette_.fill(ItemStatus::Inactive),
                 .countur = palette_.countur(ItemStatus::Inactive)});
  }
  port_.notify();
}

} // namespace Kernel
} // namespace QApp
