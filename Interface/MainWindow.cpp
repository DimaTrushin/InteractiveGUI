#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace QApp {
namespace Interface {

MainWindow::MainWindow() : ui(std::make_unique<Ui::MainWindow>()) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() = default;
} // namespace Interface
} // namespace QApp
