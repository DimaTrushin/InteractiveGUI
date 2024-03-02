#pragma once

#include "Kernel/DrawData.h"
#include "Library/Observer3/Observer.h"
#include "MouseAction.h"

#include <QObject>

#include <optional>

class QwtPlot;
class QwtPlotPicker;

namespace QApp {
namespace Interface {

class View : public QObject {
  Q_OBJECT
  using DrawData = Kernel::DrawData;
  using Data = std::optional<DrawData>;
  using ByReference = NSLibrary::CByReference;
  using ObserverState = NSLibrary::CObserver<Data, ByReference>;

  using MouseData = std::optional<MouseAction>;
  using ObservableMouse = NSLibrary::CObservableDataMono<MouseData>;
  using ObserverMouse = NSLibrary::CObserver<MouseData>;

public:
  View(QwtPlot* plot);
  ObserverState* port();

  void subscribe(ObserverMouse* obs);

private Q_SLOTS:
  void mousePressed(const QPointF& pos);
  void mouseMoved(const QPointF& pos);
  void mouseReleased(const QPointF& pos);

private:
  static void adjustPlot(QwtPlot*);
  void setPicker(QwtPlotPicker*);

  void drawData(const Data& data);
  void clear();
  void draw(const DrawData& data);
  void addItem(const DrawData::Item& item);

  QwtPlot* plot_;
  QwtPlotPicker* picker_;
  ObserverState in_port_;
  ObservableMouse out_port_;
};

} // namespace Interface
} // namespace QApp
