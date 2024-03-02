#include "FieldModel.h"

#include <cassert>

namespace QApp {
namespace Kernel {

FieldModel::FieldModel() : field_(std::in_place_t()) {
}

void FieldModel::subscribe(ObserverField* obs) {
  assert(obs);
  port_.subscribe(obs);
}

} // namespace Kernel
} // namespace QApp