#pragma once

#include <QColor>
#include <QPointF>

namespace QApp {
namespace Kernel {

struct DrawData {
  struct FieldData {
    int rows = 3;
    int columns = 3;
    QPointF origin = {0., 0.};
  };
  struct Item {
    QPointF center = {0.5, 0.5};
    double radius = 0.4;
    QColor fill = Qt::red;
    QColor countur = Qt::blue;
  };

  FieldData field;
  Item item1;
  Item item2 = {.center = {1.5, 1.5}, .fill = {Qt::green}};
};

} // namespace Kernel
} // namespace QApp
