#include "sgui/WidgetContainer.h"

#include "core/event/MouseButtonEvent.h"
#include "core/event/MouseMotionEvent.h"
#include "sgui/Widget.h"

#include <algorithm>

namespace lib
{
namespace sgui
{

WidgetContainer::~WidgetContainer()
{
    ClearWidgets();
}

void WidgetContainer::ClearWidgets()
{
    // nothing to do if empty
    if(mWidgets.empty())
        return ;

    // make a copy of the list of Widgets because deleted ones will remove themselves
    // from the original list
    std::vector<Widget *> widgets = mWidgets;
    auto it = widgets.rbegin();

    while(it != widgets.rend())
    {
        delete *it;
        ++it;
    }

    mWidgets.clear();
}

void WidgetContainer::AddChild(Widget * w)
{
    mWidgets.emplace_back(w);
}

void WidgetContainer::RemoveChild(Widget * w)
{
    auto res = std::find(mWidgets.begin(), mWidgets.end(), w);

    mWidgets.erase(res);
}

void WidgetContainer::HandleChildEnableChanged(Widget * /*child*/)
{
    // TODO - handle enable changed in child
}

void WidgetContainer::HandleChildVisibleChanged(Widget * /*child*/)
{
    // TODO - handle visible changed in child
}

void WidgetContainer::PropagateMouseButtonDown(core::MouseButtonEvent & event)
{
    const int x = event.GetX();
    const int y = event.GetY();

    for(Widget * w : mWidgets)
    {
        if(w->IsEnabled() && w->IsVisible()) // TODO remove this when implemented rendering and active lists
        {
            if(w->IsScreenPointInside(x, y))
            {
                w->PropagateMouseButtonDown(event);
                w->HandleMouseButtonDown(event);

                // stop propagation if event is consumed
                if(event.IsConsumed())
                    break;
            }
        }
    }
}

void WidgetContainer::PropagateMouseButtonUp(core::MouseButtonEvent & event)
{
    const int x = event.GetX();
    const int y = event.GetY();

    for(Widget * w : mWidgets)
    {
        if(w->IsEnabled() && w->IsVisible()) // TODO remove this when implemented rendering and active lists
        {
            if(w->IsScreenPointInside(x, y))
            {
                w->PropagateMouseButtonUp(event);
                w->HandleMouseButtonUp(event);

                // stop propagation if event is consumed
                if(event.IsConsumed())
                    break;
            }
        }
    }
}

void WidgetContainer::PropagateMouseMotion(core::MouseMotionEvent & event)
{
    const int x = event.GetX();
    const int y = event.GetY();

    for(Widget * w : mWidgets)
    {
        if(w->IsEnabled() && w->IsVisible()) // TODO remove this when implemented rendering and active lists
        {
            if(w->IsScreenPointInside(x, y))
            {
                w->PropagateMouseMotion(event);

                w->SetMouseOver();

                w->HandleMouseMotion(event);

                // stop propagation if event is consumed
                if(event.IsConsumed())
                    break;
            }
            else
                w->SetMouseOut();
        }
    }
}

void WidgetContainer::PropagateRender()
{
    for(Widget * w : mWidgets)
    {
        if(w->IsVisible())  // TODO remove this when implemented rendering lists
        {
            w->OnRender();
            w->PropagateRender();
        }
    }
}

} // namespace sgui
} // namespace lib
