#include "ApplicationImpl.h"

namespace QApp {

ApplicationImpl::ApplicationImpl()
    : controller_(&geom_model_), field_controller_(&model_) {
  model_.subscribe(geom_model_.port());
  geom_model_.subscribeToDrawData(view_.port());
  view_.subscribe(controller_.port());
  geom_model_.subscribeToItemAction(field_controller_.port());
}

} // namespace QApp
