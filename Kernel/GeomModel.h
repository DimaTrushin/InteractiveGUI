#pragma once

#include "DrawData.h"
#include "Library/Observer3/Observer.h"

#include <QTimer>

#include <optional>

namespace QApp {
namespace Kernel {

class GeomModel : public QObject {
  Q_OBJECT
  using ByReference = NSLibrary::CByReference;
  using Data = std::optional<DrawData>;
  using Observable = NSLibrary::CObservable<Data, ByReference>;
  using Observer = NSLibrary::CObserver<Data, ByReference>;

public:
  GeomModel();
  void subscribe(Observer* obs);

private Q_SLOTS:
  void onTimer();

private:
  static constexpr std::chrono::seconds k_default_timeout =
      std::chrono::seconds(1);

  std::chrono::seconds timeout_ = k_default_timeout;
  QTimer timer_;
  Data data_ = DrawData{};
  Observable port_ = [this]() -> const std::optional<DrawData>& {
    return data_;
  };
};

} // namespace Kernel
} // namespace QApp
