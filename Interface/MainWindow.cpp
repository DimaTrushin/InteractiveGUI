#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGridLayout>
#include <qwt_plot.h>

namespace QApp {
namespace Interface {

MainWindow::MainWindow() : ui_(std::make_unique<Ui::MainWindow>()) {
  ui_->setupUi(this);
  QGridLayout* gridLayout = new QGridLayout(ui_->centralwidget);
  gridLayout->addWidget(view_.plot(), 0, 0);
  gridLayout->addWidget(view2_.plot(), 0, 1);
  gridLayout->addWidget(viewg_.plot(), 1, 0);
}

View* MainWindow::view() {
  return &view_;
}

View* MainWindow::viewg() {
  return &viewg_;
}

View* MainWindow::view2() {
  return &view2_;
}

MainWindow::~MainWindow() = default;
} // namespace Interface
} // namespace QApp
