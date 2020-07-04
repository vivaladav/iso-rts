#pragma once

#include <sgui/Widget.h>

#include <functional>
#include <string>

namespace lib
{
    namespace sgui
    {
        class Label;
        class PushButton;
    }
}

namespace game
{

class ButtonsPanel;
class Player;

struct GameMapCell;

class PanelPlayer : public lib::sgui::Widget
{
public:
    PanelPlayer(Player * player, lib::sgui::Widget * parent = nullptr);

    void SetPanelCellVisible(bool val);
    void SetPanelUnitsVisible(bool val);

    void UpdateCells(int cells);
    void UpdateCoins(int coins);
    void UpdateUnits(int units);

    void SetSelectedCell(const GameMapCell & cell);

    void UpdateButtonCellFortify(int fortLevel);
    void UpdateButtonCellUpgrade(int cellLevel);

    void SetFunctionCellFortify(const std::function<void()> & f);
    void SetFunctionCellUpgrade(const std::function<void()> & f);
    void SetFunctionNewUnit(const std::function<void()> & f);

private:
    std::string MakeStrCells(int cells);
    std::string MakeStrCoins(int coins);
    std::string MakeStrUnits(int units);

    void CreatePanelCell();

    void CreatePanelUnits();

private:
    ButtonsPanel * mPanelCell = nullptr;
    ButtonsPanel * mPanelUnits = nullptr;

    lib::sgui::Label * mLabelCells = nullptr;
    lib::sgui::Label * mLabelCoins = nullptr;
    lib::sgui::Label * mLabelUnits = nullptr;

    lib::sgui::PushButton * mButtonCellFortify = nullptr;
    lib::sgui::PushButton * mButtonCellUpgrade = nullptr;
    lib::sgui::PushButton * mButtonNewUnit = nullptr;
};

} // namespace game
