#pragma once

#include "sgui/Widget.h"

#include <functional>
#include <string>
#include <vector>

namespace lib
{

namespace graphic
{
    class Font;
    class Renderable;
}

namespace sgui
{

class PushButton : public Widget
{
public:
    PushButton(Widget * parent = nullptr);
    ~PushButton();

    void SetBackground(const char * file);

    void SetLabel(const char * text);
    void SetLabelColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void SetLabelColor(unsigned int color);
    void SetLabelFont(graphic::Font * font);

    bool IsCheckable() const;
    void SetCheckable(bool val);
    bool IsChecked() const;
    void SetChecked(bool val);

    void Click();

    void AddOnClickFunction(const std::function<void()> & f);
    void AddOnToggleFunction(const std::function<void(bool)> & f);

    const std::string & GetText() const;

protected:
    void SetCurrBg(graphic::Renderable * bg);
    void SetCurrLabel(graphic::Renderable * label);

    void HandleMouseButtonUp(core::MouseButtonEvent & event) override;

    void HandlePositionChanged() override;

    void OnRender() override;

    virtual void HandleCheckedChanged(bool checked);

private:
    void PositionLabel();

private:
    std::string mText;

    std::vector<std::function<void()>> mOnClick;
    std::vector<std::function<void(bool)>> mOnToggle;

    graphic::Font * mFontLabel = nullptr;

    // actual objects created by SetBackground and SetLabel
    graphic::Renderable * mBg = nullptr;
    graphic::Renderable * mLabel = nullptr;

    // pointers to the current active objects
    graphic::Renderable * mCurrBg = nullptr;
    graphic::Renderable * mCurrLabel = nullptr;

    bool mCheckable = false;
    bool mChecked = false;
};

inline bool PushButton::IsCheckable() const { return mCheckable; }
inline void PushButton::SetCheckable(bool val)
{
    mCheckable = val;
    mChecked = false;
}
inline bool PushButton::IsChecked() const { return mChecked; }

inline void PushButton::AddOnClickFunction(const std::function<void()> & f)
{
    mOnClick.push_back(f);
}
inline void PushButton::AddOnToggleFunction(const std::function<void(bool)> & f)
{
    mOnToggle.push_back(f);
}

inline const std::string & PushButton::GetText() const { return mText; }

} // namespace sgui
} // namespace lib
