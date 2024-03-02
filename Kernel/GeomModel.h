#pragma once

#include "DrawData.h"
#include "Field.h"
#include "ItemAction.h"
#include "Library/Observer3/Observer.h"
#include "MouseAction.h"

#include <optional>

namespace QApp {
namespace Kernel {

class GeomModel {

  using ByReference = NSLibrary::CByReference;
  using Data = std::optional<DrawData>;
  using Observable = NSLibrary::CObservable<Data, ByReference>;
  using Observer = NSLibrary::CObserver<Data, ByReference>;

  using FieldData = std::optional<Field>;
  using ObserverField = NSLibrary::CObserver<FieldData>;
  using FieldDataArg = ObserverField::CArg;

  using ItemData = std::optional<ItemAction>;
  using ObservableAction = NSLibrary::CObservableDataMono<ItemData>;
  using ObserverAction = NSLibrary::CObserver<ItemData>;
  using ItemDataSent = ObserverAction::CArg;

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

  void onFieldData(FieldDataArg data);

  static constexpr int k_non = -1;
  static constexpr double k_hight = 1.;
  static constexpr double k_width = 1.;

  Data data_;
  int active_index_ = k_non;
  QPointF diff_ = {0., 0.};
  Observable port_ = [this]() -> const Data& { return data_; };
  ObserverField in_port_;
  ObservableAction action_port_;
};

} // namespace Kernel
} // namespace QApp
