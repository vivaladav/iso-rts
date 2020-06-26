#pragma once

#include <sgui/Widget.h>

#include <string>

namespace lib
{
    namespace sgui
    {
        class Label;
    }
}

namespace game
{

class Player;

class PanelPlayer : public lib::sgui::Widget
{
public:
    PanelPlayer(Player * p, lib::sgui::Widget * parent = nullptr);

    void SetPanelCellVisible(bool val);
    void SetPanelUnitsVisible(bool val);

    void UpdateCells();
    void UpdateCoins();
    void UpdateUnits();

private:
    std::string MakeStrCells();
    std::string MakeStrCoins();
    std::string MakeStrUnits();

    void CreatePanelCell();

    void CreatePanelUnits();

private:
    Player * mPlayer = nullptr;

    lib::sgui::Label * mLabelCells = nullptr;
    lib::sgui::Label * mLabelCoins = nullptr;
    lib::sgui::Label * mLabelUnits = nullptr;

    lib::sgui::Widget * mPanelCell = nullptr;
    lib::sgui::Widget * mPanelUnits = nullptr;
};

inline void PanelPlayer::SetPanelCellVisible(bool val) { mPanelCell->SetVisible(val); }
inline void PanelPlayer::SetPanelUnitsVisible(bool val) { mPanelUnits->SetVisible(val); }

} // namespace game
