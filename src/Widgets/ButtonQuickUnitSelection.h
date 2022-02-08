#pragma once

#include <sgl/sgui/ImageButton.h>

namespace lib
{
    namespace graphic
    {
        class Image;
        class Text;
    }
}

namespace game
{

class DigitsDisplay;
class ScreenGame;
class Unit;

class ButtonQuickUnitSelection : public lib::sgui::ImageButton
{
public:
    ButtonQuickUnitSelection(int index, ScreenGame * sg, lib::sgui::Widget * parent = nullptr);

    void ClearUnit();
    Unit * GetUnit() const;
    void SetUnit(Unit * unit);

private:
    void OnStateChanged(lib::sgui::PushButton::VisualState state) override;

    void HandlePositionChanged() override;

    void HandleKeyDown(lib::core::KeyboardEvent & event) override;
    void HandleKeyUp(lib::core::KeyboardEvent & event) override;

    void UpdateValues();

private:
    Unit * mUnit = nullptr;

    ScreenGame * mScreenGame = nullptr;

    lib::graphic::Text * mShortcut = nullptr;
    lib::graphic::Image * mImgUnit = nullptr;
    lib::graphic::Image * mIconEnergy = nullptr;
    DigitsDisplay * mDisplayEnergy = nullptr;
    lib::graphic::Image * mIconHealth = nullptr;
    DigitsDisplay * mDisplayHealth = nullptr;

    int mShortcutKey = -1;
};

inline Unit * ButtonQuickUnitSelection::GetUnit() const { return mUnit; }

} // namespace game
