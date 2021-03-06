#pragma once

#include "sgui/Widget.h"

#include <functional>
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

    typedef std::function<void(unsigned int, bool)> ToggleFun;

public:
    ButtonsGroup(Orientation orient, Widget * parent = nullptr);

    bool IsExclusive() const;
    void SetExclusive(bool val);

    int GetIndexChecked() const;
    void SetButtonChecked(unsigned int index, bool val);

    void SetButtonEnabled(unsigned int index, bool val);

    void AddButton(PushButton * button);

    unsigned int GetNumButtons() const;

    void SetFunctionOnToggle(const ToggleFun & f);

private:
    std::vector<PushButton *> mButtons;

    std::function<void(unsigned int, bool)> mOnToggle;

    Orientation mOrient;

    int mIndChecked = -1;

    bool mExclusive = true;
};

inline int ButtonsGroup::GetIndexChecked() const{ return mIndChecked; }

inline unsigned int ButtonsGroup::GetNumButtons() const { return mButtons.size(); }

inline void ButtonsGroup::SetFunctionOnToggle(const ToggleFun & f) { mOnToggle = f; }

} // namespace sgui
} // namespace lib
