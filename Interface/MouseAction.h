#pragma once

#include <QPointF>

namespace QApp {
namespace Interface {

enum class EMouseStatus { Pressed, Moved, Released };

struct MouseAction {
  EMouseStatus status;
  QPointF position;
};

} // namespace Interface
} // namespace QApp
