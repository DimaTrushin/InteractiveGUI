#include "View.h"

#include <QPainterPath>
#include <QPen>
#include <qwt_picker_machine.h>
#include <qwt_plot.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_shapeitem.h>

#include <cassert>

namespace QApp {
namespace Interface {

View::View(QwtPlot* plot)
    : plot_(plot), picker_(new QwtPlotPicker(plot->canvas())),
      in_port_([this](const Data& data) { drawData(data); },
               [this](const Data& data) { drawData(data); },
               ObserverState::doNothing) {
  assert(plot_);
  adjustPlot(plot_);
  setPicker(picker_);
}

View::ObserverState* View::port() {
  return &in_port_;
}

void View::subscribe(ObserverMouse* obs) {
  assert(obs);
  out_port_.subscribe(obs);
}

void View::mousePressed(const QPointF& pos) {
  out_port_.set(std::in_place_t(), EMouseStatus::Pressed, pos);
}

void View::mouseMoved(const QPointF& pos) {
  out_port_.set(std::in_place_t(), EMouseStatus::Moved, pos);
}

void View::mouseReleased(const QPointF& pos) {
  out_port_.set(std::in_place_t(), EMouseStatus::Released, pos);
}

void View::adjustPlot(QwtPlot* plot) {
  plot->setAutoDelete(true);
  plot->setAxisScale(QwtAxis::YLeft, -1, 10, 1);
  plot->setAxisScale(QwtAxis::XBottom, -1, 10, 1);
}

void View::setPicker(QwtPlotPicker* picker) {
  picker->setStateMachine(new QwtPickerDragPointMachine);
  QObject::connect(picker, &QwtPlotPicker::appended, this, &View::mousePressed);
  QObject::connect(picker, &QwtPlotPicker::moved, this, &View::mouseMoved);
  QObject::connect(picker, qOverload<const QPointF&>(&QwtPlotPicker::selected),
                   this, &View::mouseReleased);
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
