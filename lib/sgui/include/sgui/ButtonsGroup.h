#pragma once

#include "sgui/Widget.h"

#include <vector>

namespace lib
{
namespace sgui
{

class PushButton;

class ButtonsGroup : public Widget
{
public:
    enum Orientation
    {
        HORIZONTAL,
        VERTICAL
    };

public:
    ButtonsGroup(Orientation orient, Widget * parent = nullptr);

    bool IsExclusive() const;
    void SetExclusive(bool val);

    void AddButton(PushButton * button);

    unsigned int GetNumButtons() const;

private:
    std::vector<PushButton *> mButtons;

    Orientation mOrient;

    bool mExclusive = true;
};

inline unsigned int ButtonsGroup::GetNumButtons() const { return mButtons.size(); }

} // namespace sgui
} // namespace lib
