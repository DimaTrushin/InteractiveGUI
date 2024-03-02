#pragma once

#include "DrawData.h"
#include "Library/Observer3/Observer.h"

#include <QTimer>

#include <optional>

namespace QApp {
namespace Kernel {

class GeomModel : public QObject {
  Q_OBJECT
  using Observable =
      NSLibrary::CObservable<std::optional<DrawData>, NSLibrary::CByReference>;
  using Observer =
      NSLibrary::CObserver<std::optional<DrawData>, NSLibrary::CByReference>;

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
  std::optional<DrawData> data_;
  Observable port_ = [this]() -> const std::optional<DrawData>& {
    return data_;
  };
};

} // namespace Kernel
} // namespace QApp
