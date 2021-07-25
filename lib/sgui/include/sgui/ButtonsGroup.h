#pragma once

#include "sgui/AbstractButtonsGroup.h"
#include "sgui/Widget.h"

namespace lib
{
namespace sgui
{

class PushButton;

class ButtonsGroup : public AbstractButtonsGroup, public Widget
{
public:
    enum Orientation
    {
        HORIZONTAL,
        VERTICAL
    };

public:
    ButtonsGroup(Orientation orient, Widget * parent = nullptr);

    void OnButtonAdded(PushButton * button) override;

private:
    Orientation mOrient;
};

} // namespace sgui
} // namespace lib
