#pragma once

#include "ItemAction.h"
#include "QObserver.h"

#include <optional>

namespace QApp {
namespace Kernel {

class FieldModel;

class FieldController {
  using ItemData = std::optional<ItemAction>;
  using Observer = Library::CObserver<ItemData>;
  using ObserverPtr = Observer::CPointer;

public:
  FieldController(FieldModel* model);
  ObserverPtr port();

private:
  void onItemData(ItemData&& data);
  void onItemAction(ItemAction action);

  FieldModel* host_;
  Observer port_;
};

} // namespace Kernel
} // namespace QApp
