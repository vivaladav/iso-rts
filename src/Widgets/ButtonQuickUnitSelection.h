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
    ButtonQuickUnitSelection(ScreenGame * sg, lib::sgui::Widget * parent = nullptr);

    void ClearUnit();
    Unit * GetUnit() const;
    void SetUnit(Unit * unit);

private:
    void HandlePositionChanged();

private:
    Unit * mUnit = nullptr;

    ScreenGame * mScreenGame = nullptr;

    lib::graphic::Image * mIcon = nullptr;
};

inline Unit * ButtonQuickUnitSelection::GetUnit() const { return mUnit; }

} // namespace game
