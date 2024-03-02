#pragma once

#include "Kernel/DrawData.h"
#include "Library/Observer3/Observer.h"

#include <optional>

class QwtPlot;

namespace QApp {
namespace Interface {

class View {
  using DrawData = Kernel::DrawData;
  using Data = std::optional<DrawData>;
  using ByReference = NSLibrary::CByReference;
  using Observer = NSLibrary::CObserver<Data, ByReference>;

public:
  View(QwtPlot* plot);
  Observer* port();

private:
  static void adjustPlot(QwtPlot*);

  void drawData(const Data& data);
  void clear();
  void draw(const DrawData& data);
  void addItem(const DrawData::Item& item);

  QwtPlot* plot_;
  Observer port_;
};

} // namespace Interface
} // namespace QApp
