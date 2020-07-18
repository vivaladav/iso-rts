#pragma once

#include "sgui/Widget.h"

#include <functional>
#include <string>

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

    void SetOnClickFunction(const std::function<void()> & f);
    void SetOnToggleFunction(const std::function<void(bool)> & f);

    const std::string & GetText() const;

protected:
    void SetCurrBg(graphic::Renderable * bg);
    void SetCurrLabel(graphic::Renderable * label);

    void HandleMouseButtonUp(core::MouseButtonEvent & event) override;

private:
    void HandlePositionChanged() override;

    void PositionLabel();

    void OnRender() override;

private:
    std::string mText;

    std::function<void()> mOnClick;
    std::function<void(bool)> mOnToggle;

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

inline void PushButton::SetOnClickFunction(const std::function<void()> & f) { mOnClick = f; }

inline const std::string & PushButton::GetText() const { return mText; }

} // namespace sgui
} // namespace lib
