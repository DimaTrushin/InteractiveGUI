#pragma once

#include <QMainWindow>

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

private:
  std::unique_ptr<Ui::MainWindow> ui;
};
} // namespace Interface
} // namespace QApp
