#include "View.h"

#include <QPainterPath>
#include <QPen>
#include <qwt_picker_machine.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_shapeitem.h>

#include <cassert>

namespace QApp {
namespace Interface {

namespace {

class SeriesQPointF : public QwtArraySeriesData<QPointF> {
public:
  SeriesQPointF(const QPointF& corner1, const QPointF& corner2)
      : rectangle_(corner1, corner2) {
  }
  QRectF boundingRect() const override {
    return rectangle_;
  }

private:
  QRectF rectangle_;
};

} // namespace

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
  drawField(data.field);
  for (const auto& item : data.items)
    addItem(item);
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

void View::drawField(const FieldData& field) {
  plot_->setAxisScale(QwtAxis::YLeft, -1, field.rows + 1, 1);
  plot_->setAxisScale(QwtAxis::XBottom, -1, field.columns + 1, 1);
  drawVerticalLines(field);
  drawHorizontalLines(field);
}

QVector<QPointF> View::makeVerticalPath(const FieldData& field) {
  QPointF point = field.origin;
  QPointF vertical = QPointF(point.x(), field.rows * field.hight) - point;
  QPointF horizontal = QPointF(field.width, 0.);
  QVector<QPointF> path;
  path.push_back(point);
  for (int index = 0; index < field.columns; ++index) {
    point += vertical;
    path.push_back(point);
    point += horizontal;
    path.push_back(point);
    vertical *= -1;
  }
  point += vertical;
  path.push_back(point);
  return path;
}

QVector<QPointF> View::makeHorizontalPath(const FieldData& field) {
  QPointF point = field.origin;
  QPointF vertical = QPointF(0., field.hight);
  QPointF horizontal = QPointF(field.columns * field.hight, point.y()) - point;
  QVector<QPointF> path;
  path.push_back(point);
  for (int index = 0; index < field.rows; ++index) {
    point += horizontal;
    path.push_back(point);
    point += vertical;
    path.push_back(point);
    horizontal *= -1;
  }
  point += horizontal;
  path.push_back(point);
  return path;
}

void View::addPathToPlot(const FieldData& field, QVector<QPointF>&& path) {
  std::unique_ptr<QwtPlotCurve> curve = std::make_unique<QwtPlotCurve>();
  QPointF origin = field.origin;
  QPointF corner = origin;
  corner += QPointF(field.columns * field.width, field.rows * field.hight);
  std::unique_ptr<SeriesQPointF> points =
      std::make_unique<SeriesQPointF>(origin, corner);
  points->setSamples(path);
  curve->setSamples(points.release());
  curve.release()->attach(plot_);
}

void View::drawVerticalLines(const FieldData& field) {
  QVector<QPointF> path = makeVerticalPath(field);
  addPathToPlot(field, std::move(path));
}

void View::drawHorizontalLines(const FieldData& field) {
  QVector<QPointF> path = makeHorizontalPath(field);
  addPathToPlot(field, std::move(path));
}
} // namespace Interface
} // namespace QApp
