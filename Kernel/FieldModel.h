#pragma once

#include "Field.h"
#include "ItemAction.h"
#include "Library/Observer3/Observer.h"

#include <optional>

namespace QApp {
namespace Kernel {

class FieldModel {
  using FieldData = std::optional<Field>;
  using ObservableField = NSLibrary::CObservable<FieldData>;
  using ObserverField = NSLibrary::CObserver<FieldData>;
  using FieldDataArg = ObserverField::CArg;

public:
  FieldModel();

  void subscribe(ObserverField* obs);
  void makeMove(const ItemAction& action);

private:
  FieldData field_;
  ObservableField port_ = [this]() -> FieldDataArg { return field_; };
};

} // namespace Kernel
} // namespace QApp
