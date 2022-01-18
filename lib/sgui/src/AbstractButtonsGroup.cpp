#include "sgui/AbstractButtonsGroup.h"

#include "sgui/PushButton.h"

#include <cassert>

namespace lib
{
namespace sgui
{

AbstractButtonsGroup::AbstractButtonsGroup()
    : mOnToggle([](unsigned int, bool){})
{
}

void AbstractButtonsGroup::SetButtonChecked(unsigned int index, bool val)
{
    const int ind = static_cast<int>(index);

    if(index >= mButtons.size() || ind == mIndChecked)
        return ;

    PushButton * button = mButtons[index];

    if(button->IsEnabled())
        button->SetChecked(val);
}

void AbstractButtonsGroup::SetButtonEnabled(unsigned int index, bool val)
{
    if(index < mButtons.size())
    {
        PushButton * button = mButtons[index];

        if(!val && button->IsChecked())
            button->SetChecked(false);

        button->SetEnabled(val);
    }
}

void AbstractButtonsGroup::AddButton(PushButton * button)
{
    assert(button);

    const int buttonIndex = static_cast<int>(mButtons.size());

    // if mutually exclusive group button is checkable
    button->SetCheckable(mExclusive);
    button->SetExclusive(mExclusive);

    if(mExclusive)
    {
        button->AddOnToggleFunction([this, buttonIndex](bool checked)
        {
            if(buttonIndex == mIndChecked)
                return ;

            if(checked && mIndChecked > -1)
                mButtons[mIndChecked]->SetChecked(false);

            mIndChecked = buttonIndex;

            mOnToggle(buttonIndex, checked);
        });
    }

    mButtons.emplace_back(button);

    // allow subclasses to react to add
    OnButtonAdded(button);
}

void AbstractButtonsGroup::OnButtonAdded(PushButton *) { }

} // namespace sgui
} // namespace lib
