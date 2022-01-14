#pragma once

#include "Screen.h"

#include "GameConstants.h"

#include <array>
#include <vector>

namespace lib
{
    namespace graphic { class Image; }

    namespace sgui
    {
        class PushButton;
        class Widget;
    }
}

namespace game
{

class ButtonDialogContinue;
class ButtonDialogSelect;

class ScreenFactionSelection : public Screen
{
public:
    ScreenFactionSelection(Game * game);
    ~ScreenFactionSelection();

    void Update(float update) override;
    void Render() override;

private:
    enum FactionStats : unsigned int
    {
        EXPLORATION,
        CONSTRUCTION,
        COMBAT,
        TECHNOLOGY,

        NUM_FACTION_STATS
    };

private:
    void InitSprites();

    int AddPanelStats(int x, int y, const std::array<int, NUM_FACTION_STATS> & stats,
                      lib::sgui::Widget * parent);

    void HandleSelect(bool selected, PlayerFaction faction);

private:
    std::array<lib::sgui::Widget *, NUM_FACTIONS> mColumns;
    std::array<ButtonDialogSelect *, NUM_FACTIONS> mButtonsSelect;

    lib::graphic::Image * mBg = nullptr;

    ButtonDialogContinue * mButtonCont = nullptr;

    PlayerFaction mFaction;
};

} // namespace game
