#pragma once

#include <QMainWindow>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace QApp {
namespace Interface {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

  QwtPlot* plot();

private:
  std::unique_ptr<Ui::MainWindow> ui_;
  std::unique_ptr<QwtPlot> plot_;
};
} // namespace Interface
} // namespace QApp
