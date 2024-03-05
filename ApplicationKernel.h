#pragma once

#include "Kernel/FieldModel.h"
#include "Kernel/GeomModel.h"

namespace QApp {

class ApplicationKernel {
  using FieldModel = Kernel::FieldModel;
  using GeomModel = Kernel::GeomModel;

protected:
  FieldModel model_;
  GeomModel geom_model_;
};

} // namespace QApp
