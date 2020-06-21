#pragma once

#include <sgui/Widget.h>

namespace game
{

class Player;

class PanelPlayer : public lib::sgui::Widget
{
public:
    PanelPlayer(Player * p, lib::sgui::Widget * parent = nullptr);

    void SetPanelCellVisible(bool val);
    void SetPanelUnitsVisible(bool val);

private:
    void CreatePanelCell();

    void CreatePanelUnits();

private:
    lib::sgui::Widget * mPanelCell = nullptr;
    lib::sgui::Widget * mPanelUnits = nullptr;
};

inline void PanelPlayer::SetPanelCellVisible(bool val) { mPanelCell->SetVisible(val); }
inline void PanelPlayer::SetPanelUnitsVisible(bool val) { mPanelUnits->SetVisible(val); }

} // namespace game
