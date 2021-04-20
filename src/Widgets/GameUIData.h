#pragma once

namespace game
{

// OBJECT ACTION BUTTON
extern const char SpriteFileObjActionButton[];

enum SpriteIdObjActionButton : unsigned int
{
    // BUTTON
    IND_BUTTON_NORMAL,
    IND_BUTTON_DISABLED,
    IND_BUTTON_MOUSE_OVER,
    IND_BUTTON_PUSHED,
    IND_BUTTON_CHECKED,

    // ICONS
    IND_BUTTON_ICON_UNITS,
    IND_BUTTON_ICON_MOVE,
    IND_BUTTON_ICON_ATTACK,
    IND_BUTTON_ICON_CONQUER,
    IND_BUTTON_ICON_BUILD_WALL,
    IND_BUTTON_ICON_UPGRADE,
    IND_BUTTON_ICON_CANCEL,

    NUM_IND_OBJ_ACT_BUTTON
};

} // namespace game
