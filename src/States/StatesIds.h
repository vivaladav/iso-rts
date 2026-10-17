#pragma once

namespace game
{

enum StateId : unsigned int
{
    INIT,
    INIT_GAME,
    INIT_GAME_SETUP,
    GAME,
    MAIN_MENU,
    NEW_GAME_SETUP,
    FACTION_SEL,
    LOAD_GAME,
    PLANET_MAP,
    SETTINGS,
    CREDITS,
    TEST,

    NUM_SCREENS // keep last
};

} // namespace game
