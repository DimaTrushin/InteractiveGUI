#include "ApplicationImpl.h"

#include <oneapi/tbb/info.h>

#include <QDebug>

namespace QApp {

ApplicationImpl::ApplicationImpl() {
  main_window_.show();
  int num_threads = oneapi::tbb::info::default_concurrency();
  qDebug() << "threads =" << num_threads;
}

} // namespace QApp
