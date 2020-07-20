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

void ButtonsGroup::AddButton(PushButton * button)
{
    assert(button);

    const int w = GetWidth();
    const int h = GetHeight();

    int x = GetScreenX();
    int y = GetScreenY();

    if(mOrient == HORIZONTAL)
        x += w;
    else
        y += h;

    // button's position depends on orientation
    button->SetPosition(x, y);

    // if mutually exclusive group button is checkable
    button->SetCheckable(mExclusive);

    // this will update the size of the group
    button->SetParent(this);

    mButtons.emplace_back(button);
}

} // namespace sgui
} // namespace lib
