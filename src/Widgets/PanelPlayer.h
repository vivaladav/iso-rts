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
class GameObject;
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
    void UpdateDiamonds(int diamonds);
    void UpdateUnits(int units);

    void ClearSelectedCell();
    void SetSelectedCell(const GameMapCell & cell);
    void SetSelectedObject(GameObject * obj);

    void UpdateButtonNewUnit(const GameObject * obj);
    void UpdateButtonUnitUpgrade(const GameObject * obj);
    void UpdateButtonConquer(const GameObject * obj);

    void SetFunctionCellConquest(const std::function<void()> & f);
    void SetFunctionNewUnit(const std::function<void()> & f);

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
    lib::sgui::Label * mLabelDiamonds = nullptr;
    lib::sgui::Label * mLabelUnits = nullptr;

    ButtonPanelPlayer * mButtonNewUnit = nullptr;

    ButtonPanelPlayer * mButtonUnitsUpgrade = nullptr;
    ButtonPanelPlayer * mButtonCellConquer = nullptr;

    Player * mPlayer = nullptr;
};

} // namespace game
