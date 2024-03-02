#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGridLayout>

namespace QApp {
namespace Interface {

MainWindow::MainWindow()
    : ui_(std::make_unique<Ui::MainWindow>()),
      plot_(std::make_unique<QwtPlot>()) {
  ui_->setupUi(this);
  QGridLayout* gridLayout = new QGridLayout(ui_->centralwidget);
  gridLayout->addWidget(plot_.get(), 0, 1);
  plot_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  plot_->setAxisVisible(QwtAxis::YLeft, false);
  plot_->setAxisVisible(QwtAxis::XBottom, false);
}

QwtPlot* MainWindow::plot() {
  return plot_.get();
}

MainWindow::~MainWindow() = default;
} // namespace Interface
} // namespace QApp
