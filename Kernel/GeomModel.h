#pragma once

#include "DrawData.h"
#include "Field.h"
#include "ItemAction.h"
#include "MouseAction.h"
#include "QObserver.h"

#include <optional>

namespace QApp {
namespace Kernel {

class GeomModel {

  using Data = std::optional<DrawData>;
  using Observable = Library::CObservable<Data>;
  using Observer = Library::CObserver<Data>;

  using FieldData = std::optional<Field>;
  using ObserverField = Library::CObserver<FieldData>;

  using ItemData = std::optional<ItemAction>;
  using ObservableAction = Library::CObservableDataMono<ItemData>;
  using ObserverAction = Library::CObserver<ItemData>;

public:
  GeomModel();

  void subscribeToDrawData(Observer* obs);
  void onMouseAction(const MouseAction& action);
  ObserverField* port();
  void subscribeToItemAction(ObserverAction* obs);

private:
  void onMousePress(const QPointF& position);
  void onMouseMove(const QPointF& position);
  void onMouseRelease(const QPointF& position);

  int touchedItem(const QPointF& position) const;
  int getRow(const QPointF& position) const;
  int getColumn(const QPointF& position) const;

  void onFieldData(FieldData&& data);

  static constexpr int k_non = -1;
  static constexpr double k_hight = 1.;
  static constexpr double k_width = 1.;
  static constexpr double k_item_radius = 0.4;
  static constexpr QPointF k_origin = {0., 0.};

  Data data_;
  int active_index_ = k_non;
  QPointF diff_ = {0., 0.};
  Observable port_ = [this]() -> const Data& { return data_; };
  ObserverField in_port_;
  ObservableAction action_port_;
};

} // namespace Kernel
} // namespace QApp
