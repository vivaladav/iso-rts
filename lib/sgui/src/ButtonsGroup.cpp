#include "sgui/ButtonsGroup.h"

#include "sgui/PushButton.h"

#include <cassert>

namespace lib
{
namespace sgui
{

ButtonsGroup::ButtonsGroup(Orientation orient, Widget * parent)
    : Widget(parent)
    , mOnToggle([](unsigned int, bool){})
    , mOrient(orient)
{

}

void ButtonsGroup::SetChecked(unsigned int index)
{
    const int ind = static_cast<int>(index);

    if(index >= mButtons.size() || ind == mIndChecked)
        return ;

    mIndChecked = ind;

    mButtons[index]->SetChecked(true);
}

void ButtonsGroup::AddButton(PushButton * button)
{
    assert(button);

    const int w = GetWidth();
    const int h = GetHeight();

    int x = 0;
    int y = 0;

    if(mOrient == HORIZONTAL)
        x += w;
    else
        y += h;

    const int buttonIndex = static_cast<int>(mButtons.size());

    // if mutually exclusive group button is checkable
    button->SetCheckable(mExclusive);

    if(mExclusive)
    {
        button->SetOnToggleFunction([this, buttonIndex](bool checked)
        {
            if(buttonIndex == mIndChecked)
                return ;

            if(checked && mIndChecked > -1)
                mButtons[mIndChecked]->SetChecked(false);

            mIndChecked = buttonIndex;

            mOnToggle(buttonIndex, checked);
        });
    }
    else
    {

    }

    // this will update the size of the group
    button->SetParent(this);

    // button's position depends on orientation
    button->SetPosition(x, y);

    mButtons.emplace_back(button);
}

} // namespace sgui
} // namespace lib
