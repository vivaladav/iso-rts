#pragma once

#include <sgui/Widget.h>

#include <functional>
#include <string>

namespace lib
{
    namespace sgui { class Label; }
}

namespace game
{

class ButtonPanelPlayer;
class ButtonsPanel;
class Player;
class UnitsSelector;

struct GameMapCell;

class PanelPlayer : public lib::sgui::Widget
{
public:
    enum PanelPosition : int
    {
        PPOS_TL,
        PPOS_TR,
        PPOS_BL,
        PPOS_BR,

        NUM_PANEL_POSITIONS
    };

public:
    PanelPlayer(Player * player, PanelPosition pos, lib::sgui::Widget * parent = nullptr);

    void SetPanelCellVisible(bool val);
    void SetPanelUnitsVisible(bool val);

    void UpdateCells(int cells);
    void UpdateCoins(int coins);
    void UpdateUnits(int units);

    void ClearSelectedCell();
    void SetSelectedCell(const GameMapCell & cell);

    void UpdateButtonCellFortify(int fortLevel);
    void UpdateButtonCellUpgrade(int cellLevel);
    void UpdateButtonNewUnit(int num, int level);
    void UpdateButtonUnitDestroy();
    void UpdateButtonUnitUpgrade(int num, int level);

    void SetFunctionCellConquest(const std::function<void()> & f);
    void SetFunctionCellFortify(const std::function<void()> & f);
    void SetFunctionCellUpgrade(const std::function<void()> & f);
    void SetFunctionNewUnit(const std::function<void()> & f);

    void SetFunctionUnitsDestroy(const std::function<void()> & f);
    void SetFunctionUnitsUpgrade(const std::function<void()> & f);

private:
    std::string MakeStrCells(int cells);
    std::string MakeStrCoins(int coins);
    std::string MakeStrUnits(int units);

    void CreatePanelCell(PanelPosition pos);

    void CreatePanelUnits(PanelPosition pos);

private:
    ButtonsPanel * mPanelCell = nullptr;
    ButtonsPanel * mPanelUnits = nullptr;

    lib::sgui::Label * mLabelCells = nullptr;
    lib::sgui::Label * mLabelCoins = nullptr;
    lib::sgui::Label * mLabelUnits = nullptr;

    ButtonPanelPlayer * mButtonCellFortify = nullptr;
    ButtonPanelPlayer * mButtonCellUpgrade = nullptr;
    ButtonPanelPlayer * mButtonNewUnit = nullptr;

    ButtonPanelPlayer * mButtonUnitsDestroy = nullptr;
    ButtonPanelPlayer * mButtonUnitsDestroyConf = nullptr;
    ButtonPanelPlayer * mButtonUnitsUpgrade = nullptr;
    ButtonPanelPlayer * mButtonCellConquest = nullptr;
};

} // namespace game
