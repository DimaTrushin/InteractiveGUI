#include "Application.h"
#include "Except.h"

#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication runtime(argc, argv);
  try {
    QApp::Application app;
    runtime.exec();
  } catch (...) {
    QApp::Except::react();
  }
  return 0;
}
