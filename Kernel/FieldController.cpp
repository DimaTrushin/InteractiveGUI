#include "FieldController.h"

#include "FieldModel.h"

#include <cassert>

namespace QApp {
namespace Kernel {

FieldController::FieldController(FieldModel* model)
    : host_(model), port_([this](ItemDataArg data) { onItemData(data); },
                          [this](ItemDataArg data) { onItemData(data); },
                          Observer::doNothing) {
  assert(host_);
}

FieldController::Observer* FieldController::port() {
  return &port_;
}

void FieldController::onItemData(ItemDataArg data) {
  if (data.has_value())
    onItemAction(*data);
}

void FieldController::onItemAction(ItemAction action) {
  assert(host_);
  host_->makeMove(action);
}

} // namespace Kernel
} // namespace QApp
