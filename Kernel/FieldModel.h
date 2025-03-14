#pragma once

#include "Field.h"
#include "ItemAction.h"
#include "QObserver.h"

#include <optional>

namespace QApp {
namespace Kernel {

class FieldModel {
  using FieldData = std::optional<Field>;
  using ObservableField = Library::CObservable<FieldData>;
  using ObserverField = Library::CObserver<FieldData>;
  using ObserverFieldPtr = ObserverField::CPointer;

public:
  FieldModel();

  void subscribe(ObserverFieldPtr obs);
  void makeMove(const ItemAction& action);

private:
  FieldData field_;
  ObservableField port_ = [this]() -> const FieldData& { return field_; };
};

} // namespace Kernel
} // namespace QApp
