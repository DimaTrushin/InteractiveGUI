#include "ItemAnimator.h"

#include <QDebug>

#include <cassert>
#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>

namespace QApp {
namespace Kernel {

ItemAnimator::ItemAnimator(Action f) : action_(std::move(f)) {
  assert(action_);
  QObject::connect(&timer_, &QTimer::timeout, this, &ItemAnimator::onTimer);
}

void ItemAnimator::startAnimation(const Item& item) {
  current_ = item;
  active_ = true;
  step_ = 0;
  initial_radius_ = current_.radius;
  using namespace std::chrono;
  timer_.start(40ms);
}

void ItemAnimator::stopAnimation() {
  active_ = false;
  timer_.stop();
}

void ItemAnimator::setCenter(const QPointF& center) {
  current_.center = center;
}

void ItemAnimator::onTimer() {
  if (active_) {
    double delta = 0.1 * initial_radius_ * std::sin((M_PI * step_) / 2.);
    current_.radius += delta;
    action_(current_);
    ++step_;
  }
}

} // namespace Kernel
} // namespace QApp
