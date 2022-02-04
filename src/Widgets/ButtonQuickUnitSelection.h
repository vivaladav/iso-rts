#pragma once

#include <sgui/ImageButton.h>

namespace lib
{
    namespace graphic { class Image; }
}

namespace game
{

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
    void HandlePositionChanged() override;

    void HandleKeyDown(lib::core::KeyboardEvent & event) override;
    void HandleKeyUp(lib::core::KeyboardEvent & event) override;

private:
    Unit * mUnit = nullptr;

    ScreenGame * mScreenGame = nullptr;

    lib::graphic::Image * mIcon = nullptr;

    int mShortcutKey = -1;
};

inline Unit * ButtonQuickUnitSelection::GetUnit() const { return mUnit; }

} // namespace game
