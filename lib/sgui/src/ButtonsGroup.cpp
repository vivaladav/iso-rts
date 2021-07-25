#include "sgui/ButtonsGroup.h"

#include "sgui/PushButton.h"

#include <cassert>

namespace lib
{
namespace sgui
{

ButtonsGroup::ButtonsGroup(Orientation orient, Widget * parent)
    : Widget(parent)
    , mOrient(orient)
{
}

void ButtonsGroup::OnButtonAdded(PushButton * button)
{
    const int w = GetWidth();
    const int h = GetHeight();

    // this will update the size of the group
    button->SetParent(this);

    int x = 0;
    int y = 0;

    if(mOrient == HORIZONTAL)
        x += w;
    else
        y += h;

    // button's position depends on orientation
    button->SetPosition(x, y);
}

} // namespace sgui
} // namespace lib
