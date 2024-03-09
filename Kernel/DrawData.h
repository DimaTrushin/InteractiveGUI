#pragma once

#include <QColor>
#include <QPointF>

#include <vector>

namespace QApp {
namespace Kernel {

struct DrawData {
  struct Field {
    int rows;
    int columns;
    double hight;
    double width;
  };
  struct FieldOnPlot : Field {
    QPointF origin;
  };
  struct Item {
    double radius;
    QColor fill;
    QColor countur;
  };

  struct ItemOnField : Item {
    QPointF center;
  };

  FieldOnPlot field;
  std::vector<ItemOnField> items;
};

} // namespace Kernel
} // namespace QApp
