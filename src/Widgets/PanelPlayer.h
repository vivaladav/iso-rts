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

    void ClearSelectedCell();
    void SetSelectedCell(const GameMapCell & cell);

    void UpdateButtonCellFortify(int fortLevel);
    void UpdateButtonCellUpgrade(int cellLevel);
    void UpdateButtonNewUnit(int num, int level);
    void UpdateButtonUnitUpgrade(int num, int level);

    void SetFunctionCellFortify(const std::function<void()> & f);
    void SetFunctionCellUpgrade(const std::function<void()> & f);
    void SetFunctionNewUnit(const std::function<void()> & f);

    void SetFunctionUnitsMove(const std::function<void()> & f);
    void SetFunctionUnitsUpgrade(const std::function<void()> & f);

    int GetNumUnitsToMove() const;
    void ClearNumUnitsToMove();

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

    lib::sgui::PushButton * mButtonUnitsMove = nullptr;
    lib::sgui::PushButton * mButtonUnitsUpgrade = nullptr;

    int mNumUnitToMove = 0;
};

inline int PanelPlayer::GetNumUnitsToMove() const { return mNumUnitToMove; }
inline void PanelPlayer::ClearNumUnitsToMove() { mNumUnitToMove = 0; }

} // namespace game
