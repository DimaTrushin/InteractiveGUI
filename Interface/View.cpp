#include "View.h"

#include <QPainterPath>
#include <QPen>
#include <qwt_plot.h>
#include <qwt_plot_shapeitem.h>

#include <cassert>

namespace QApp {
namespace Interface {

View::View(QwtPlot* plot)
    : plot_(plot),
      port_([this](const Data& data) { drawData(data); },
            [this](const Data& data) { drawData(data); }, Observer::doNothing) {
  assert(plot_);
  adjustPlot(plot_);
}

View::Observer* View::port() {
  return &port_;
}

void View::adjustPlot(QwtPlot* plot) {
  plot->setAxisScale(QwtAxis::YLeft, -1, 10, 1);
  plot->setAxisScale(QwtAxis::XBottom, -1, 10, 1);
}

void View::drawData(const Data& data) {
  if (data.has_value()) {
    draw(*data);
  } else {
    clear();
  }
}

void View::clear() {
  plot_->detachItems();
  plot_->replot();
}

void View::draw(const DrawData& data) {
  plot_->detachItems();
  addItem(data.item1);
  addItem(data.item2);
  plot_->replot();
}

void View::addItem(const DrawData::Item& item) {
  std::unique_ptr<QwtPlotShapeItem> plot_item =
      std::make_unique<QwtPlotShapeItem>();

  QPainterPath path;
  path.addEllipse(item.center, item.radius, item.radius);
  plot_item->setShape(path);

  QPen pen(item.countur);
  plot_item->setPen(pen);

  QBrush brush(item.fill);
  plot_item->setBrush(brush);

  plot_item.release()->attach(plot_);
}
} // namespace Interface
} // namespace QApp
