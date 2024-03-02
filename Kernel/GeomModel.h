#pragma once

#include "DrawData.h"
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

public:
  GeomModel();

  void subscribe(Observer* obs);
  void onMouseAction(const MouseAction& action);

private:
  void onMousePress(const QPointF& position);
  void onMouseMove(const QPointF& position);
  void onMouseRelease(const QPointF& position);

  int touchedItem(const QPointF& position) const;

  static constexpr int k_non = -1;

  Data data_ = DrawData{};
  int active_index_ = k_non;
  QPointF diff_ = {0., 0.};
  Observable port_ = [this]() -> const Data& { return data_; };
};

} // namespace Kernel
} // namespace QApp
