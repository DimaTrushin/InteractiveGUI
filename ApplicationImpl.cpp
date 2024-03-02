#include "ApplicationImpl.h"

#include <oneapi/tbb/info.h>

#include <QDebug>

namespace QApp {

ApplicationImpl::ApplicationImpl()
    : view_(main_window_.plot()), controller_(&geom_model_),
      field_controller_(&model_) {
  main_window_.show();
  int num_threads = oneapi::tbb::info::default_concurrency();
  qDebug() << "threads =" << num_threads;
  model_.subscribe(geom_model_.port());
  geom_model_.subscribeToDrawData(view_.port());
  view_.subscribe(controller_.port());
  geom_model_.subscribeToItemAction(field_controller_.port());
}

} // namespace QApp
