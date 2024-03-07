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
    double hight;
    double width;
    // need to fix the origin
    QPointF origin = {0., 0.};
  };
  struct Item {
    QPointF center;
    double radius;
    QColor fill = Qt::red;
    QColor countur = Qt::blue;
  };

  FieldData field;
  std::vector<Item> items;
};

} // namespace Kernel
} // namespace QApp
