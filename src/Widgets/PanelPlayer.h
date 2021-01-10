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
    PanelPlayer(Player * player, lib::sgui::Widget * parent = nullptr);

    void SetPanelCellVisible(bool val);
    void SetPanelUnitsVisible(bool val);

    void UpdateCells(int cells);
    void UpdateEnergy(int energy);
    void UpdateMaterial(int material);
    void UpdateUnits(int units);

    void ClearSelectedCell();
    void SetSelectedCell(const GameMapCell & cell);

    void UpdateButtonNewUnit(const GameMapCell & cell);
    void UpdateButtonUnitDestroy();
    void UpdateButtonUnitUpgrade();
    void UpdateButtonConquer(const GameMapCell & cell);

    void SetFunctionCellConquest(const std::function<void()> & f);
    void SetFunctionNewUnit(const std::function<void()> & f);

    void SetFunctionUnitsDestroy(const std::function<void()> & f);
    void SetFunctionUnitsUpgrade(const std::function<void()> & f);

private:
    std::string MakeStrValue(int value);

    void CreatePanelCell();

    void CreatePanelUnits();

private:
    ButtonsPanel * mPanelCell = nullptr;
    ButtonsPanel * mPanelUnits = nullptr;

    lib::sgui::Label * mLabelCells = nullptr;
    lib::sgui::Label * mLabelEnergy = nullptr;
    lib::sgui::Label * mLabelMaterial = nullptr;
    lib::sgui::Label * mLabelUnits = nullptr;

    ButtonPanelPlayer * mButtonNewUnit = nullptr;

    ButtonPanelPlayer * mButtonUnitsDestroy = nullptr;
    ButtonPanelPlayer * mButtonUnitsDestroyConf = nullptr;
    ButtonPanelPlayer * mButtonUnitsUpgrade = nullptr;
    ButtonPanelPlayer * mButtonCellConquer = nullptr;

    Player * mPlayer = nullptr;
};

} // namespace game
