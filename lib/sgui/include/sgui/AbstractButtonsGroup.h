#pragma once

#include <functional>
#include <vector>

namespace lib
{
namespace sgui
{

class PushButton;

class AbstractButtonsGroup
{
public:
    typedef std::function<void(unsigned int, bool)> ToggleFun;

public:
    AbstractButtonsGroup();

    bool IsExclusive() const;
    void SetExclusive(bool val);

    int GetIndexChecked() const;
    void SetButtonChecked(unsigned int index, bool val);

    void SetButtonEnabled(unsigned int index, bool val);

    void AddButton(PushButton * button);
    PushButton * GetButton(unsigned int ind) const;

    unsigned int GetNumButtons() const;

    void SetFunctionOnToggle(const ToggleFun & f);

private:
    virtual void OnButtonAdded(PushButton * button);

private:
    std::vector<PushButton *> mButtons;

    std::function<void(unsigned int, bool)> mOnToggle;

    int mIndChecked = -1;

    bool mExclusive = true;
};

inline int AbstractButtonsGroup::GetIndexChecked() const{ return mIndChecked; }

inline PushButton * AbstractButtonsGroup::GetButton(unsigned int ind) const
{
    if(ind < mButtons.size())
        return mButtons[ind];
    else
        return nullptr;
}

inline unsigned int AbstractButtonsGroup::GetNumButtons() const { return mButtons.size(); }

inline void AbstractButtonsGroup::SetFunctionOnToggle(const ToggleFun & f) { mOnToggle = f; }

} // namespace sgui
} // namespace lib
