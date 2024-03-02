#include "ApplicationImpl.h"

#include <oneapi/tbb/info.h>

#include <QDebug>

namespace QApp {

ApplicationImpl::ApplicationImpl() : view_(main_window_.plot()) {
  main_window_.show();
  int num_threads = oneapi::tbb::info::default_concurrency();
  qDebug() << "threads =" << num_threads;
  model_.subscribe(view_.port());
}

} // namespace QApp
