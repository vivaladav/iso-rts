#include "ButtonsPanel.h"

#include <core/event/MouseButtonEvent.h>

namespace game
{

// Member that consumes a mouse up event when a disabled element is clicked.
// This is useful to avoid that clicking a disabled button clears the current
// cell selection
void ButtonsPanel::HandleMouseButtonUp(lib::core::MouseButtonEvent & event)
{
    const int x = event.GetX();
    const int y = event.GetY();

    for(lib::sgui::Widget * w : mWidgets)
    {
        if(!w->IsEnabled() && w->IsScreenPointInside(x, y))
        {
            event.SetConsumed();
            break;
        }
    }
}

} // namespace game
