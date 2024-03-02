#pragma once

#include <QColor>
#include <QPointF>

#include <vector>

namespace QApp {
namespace Kernel {

struct DrawData {
  struct FieldData {
    int rows;
    int columns;
    double hight = 1.;
    double width = 1.;
    QPointF origin = {0., 0.};
  };
  struct Item {
    QPointF center = {0.5, 0.5};
    double radius = 0.4;
    QColor fill = Qt::red;
    QColor countur = Qt::blue;
  };

  FieldData field;
  std::vector<Item> items;
};

} // namespace Kernel
} // namespace QApp
