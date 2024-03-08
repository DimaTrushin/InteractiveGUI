#pragma once

#include "DrawData.h"

#include <QTimer>

#include <functional>

namespace QApp {
namespace Kernel {

class ItemAnimator : public QObject {
  Q_OBJECT
  using Item = DrawData::Item;
  using Action = std::function<void(const Item&)>;

public:
  ItemAnimator(Action f);
  ItemAnimator(const ItemAnimator&) = delete;
  ItemAnimator(ItemAnimator&&) noexcept = delete;
  ItemAnimator& operator=(const ItemAnimator&) = delete;
  ItemAnimator& operator=(ItemAnimator&&) noexcept = delete;

  void startAnimation(const Item&);
  void stopAnimation();

  void setCenter(const QPointF&);

private Q_SLOTS:
  void onTimer();

private:
  Action action_;
  Item current_;
  bool active_ = false;
  int step_ = 0;
  double initial_radius_ = 0.;
  QTimer timer_;
};

} // namespace Kernel
} // namespace QApp