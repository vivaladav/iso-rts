#pragma once

#include "Screen.h"

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

enum PlayerFaction : unsigned int;

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

    void AddPanelStats(int x, int y, const std::array<int, NUM_FACTION_STATS> & stats,
                       lib::sgui::Widget * parent);

private:
    lib::graphic::Image * mBg = nullptr;

    PlayerFaction mFaction;
};

} // namespace game
