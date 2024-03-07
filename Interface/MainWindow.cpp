#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGridLayout>
#include <qwt_plot.h>

namespace QApp {
namespace Interface {

MainWindow::MainWindow() : ui_(std::make_unique<Ui::MainWindow>()) {
  ui_->setupUi(this);
  QGridLayout* gridLayout = new QGridLayout(ui_->centralwidget);
  gridLayout->addWidget(view_.plot(), 0, 1);
}

View* MainWindow::view() {
  return &view_;
}

MainWindow::~MainWindow() = default;
} // namespace Interface
} // namespace QApp
