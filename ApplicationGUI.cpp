#include "ApplicationGUI.h"

namespace QApp {

ApplicationGUI::ApplicationGUI() : view_(main_window_.plot()) {
  main_window_.show();
}

} // namespace QApp
