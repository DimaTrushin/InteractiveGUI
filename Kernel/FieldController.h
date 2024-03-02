#pragma once

#include "ItemAction.h"
#include "Library/Observer3/Observer.h"

#include <optional>

namespace QApp {
namespace Kernel {

class FieldModel;

class FieldController {
  using ItemData = std::optional<ItemAction>;
  using Observer = NSLibrary::CObserver<ItemData>;
  using ItemDataArg = Observer::CArg;

public:
  FieldController(FieldModel* model);
  Observer* port();

private:
  void onItemData(ItemDataArg data);
  void onItemAction(ItemAction action);

  FieldModel* host_;
  Observer port_;
};

} // namespace Kernel
} // namespace QApp
