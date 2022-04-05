#pragma once

namespace game
{

// NEW ELEMENT DIALOG
extern const char SpriteFileNewElementDialog[];

enum SpriteIdNewElementDialog : unsigned int
{
    // BACKGROUND
    IND_NE_DIALOG_BG,

    // PANELS
    IND_NE_DIALOG_ATT_ON,
    IND_NE_DIALOG_ATT_OFF,
    IND_NE_DIALOG_INFO,
    IND_NE_DIALOG_PANEL_NORMAL,
    IND_NE_DIALOG_PANEL_OVER,
    IND_NE_DIALOG_PANEL_SEL,
    IND_NE_DIALOG_PANEL_DISABLED,

    // ATTRIBUTES BAR
    IND_NE_DIALOG_BAR0,
    IND_NE_DIALOG_BAR1,
    IND_NE_DIALOG_BAR2,
    IND_NE_DIALOG_BAR3,
    IND_NE_DIALOG_BAR4,
    IND_NE_DIALOG_BAR5,
    IND_NE_DIALOG_BAR6,
    IND_NE_DIALOG_BAR7,
    IND_NE_DIALOG_BAR8,
    IND_NE_DIALOG_BAR9,
    IND_NE_DIALOG_BAR10,

    // BUTTON CLOSE
    IND_NE_DIALOG_CLOSE_NORMAL,
    IND_NE_DIALOG_CLOSE_OVER,
    IND_NE_DIALOG_CLOSE_PUSHED,

    // BUTTON BUILD
    IND_NE_DIALOG_BUILD_NORMAL,
    IND_NE_DIALOG_BUILD_OVER,
    IND_NE_DIALOG_BUILD_PUSHED,
    IND_NE_DIALOG_BUILD_DISABLED,

    // ICONS
    IND_NE_DIALOG_ICON_ENERGY,
    IND_NE_DIALOG_ICON_MATERIAL,
    IND_NE_DIALOG_ICON_DIAMOND,
    IND_NE_DIALOG_ICON_BLOB,

    // BUTTON LEFT
    IND_NE_DIALOG_LEFT_NORMAL,
    IND_NE_DIALOG_LEFT_DISABLED,
    IND_NE_DIALOG_LEFT_OVER,
    IND_NE_DIALOG_LEFT_PUSHED,

    // BUTTON RIGHT
    IND_NE_DIALOG_RIGHT_NORMAL,
    IND_NE_DIALOG_RIGHT_DISABLED,
    IND_NE_DIALOG_RIGHT_OVER,
    IND_NE_DIALOG_RIGHT_PUSHED
};

// FACTION SELECTION DIALOG
extern const char SpriteFileFactionSelection[];

enum SpriteIdFactionSelection : unsigned int
{
    // PANELS
    IND_FSEL_MAIN_BG,
    IND_FSEL_PANEL_F1,
    IND_FSEL_PANEL_F2,
    IND_FSEL_PANEL_F3,
    IND_FSEL_PANEL_TXT,
    IND_FSEL_PANEL_ATTR,

    // ATTRIBUTES BAR
    IND_FSEL_BAR0,
    IND_FSEL_BAR1,
    IND_FSEL_BAR2,
    IND_FSEL_BAR3,
    IND_FSEL_BAR4,
    IND_FSEL_BAR5,
    IND_FSEL_BAR6,
    IND_FSEL_BAR7,
    IND_FSEL_BAR8,
    IND_FSEL_BAR9,
    IND_FSEL_BAR10,

    // BACK BUTTON
    IND_FSEL_BACK_NORMAL,
    IND_FSEL_BACK_OVER,
    IND_FSEL_BACK_PUSHED,

    // SELECT BUTTON
    IND_FSEL_SEL_NORMAL,
    IND_FSEL_SEL_OVER,
    IND_FSEL_SEL_PUSHED,
    IND_FSEL_SEL_CHECKED,

    // CONTINUE BUTTON
    IND_FSEL_CONTINUE_NORMAL,
    IND_FSEL_CONTINUE_DISABLED,
    IND_FSEL_CONTINUE_OVER,
    IND_FSEL_CONTINUE_PUSHED,

    // SPECIAL VALUES
    NUM_FSEL_PIPS = IND_FSEL_BAR9 - IND_FSEL_BAR0 + 1
};

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
    IND_BUTTON_ICON_CONQUER_CELL,
    IND_BUTTON_ICON_BUILD_WALL,
    IND_BUTTON_ICON_BUILD_STRUCT,
    IND_BUTTON_ICON_UPGRADE,
    IND_BUTTON_ICON_OPEN_GATE,
    IND_BUTTON_ICON_CLOSE_GATE,
    IND_BUTTON_ICON_CANCEL,

    NUM_IND_OBJ_ACT_BUTTON
};

// MAIN MENU BUTTONS
extern const char SpriteFileMainMenu[];

enum SpriteIdMainMenu : unsigned int
{
    // PRIMARY BUTTON
    IND_MM_BTN1_NORMAL,
    IND_MM_BTN1_DISABLED,
    IND_MM_BTN1_OVER,
    IND_MM_BTN1_PUSHED,
    IND_MM_BTN1_CHECKED,

    // WISHLIST BUTTON
    IND_MM_BTNW_NORMAL,
    IND_MM_BTNW_OVER,
    IND_MM_BTNW_PUSHED,

    // SOCIAL BUTTON
    IND_MM_BTNS_NORMAL,
    IND_MM_BTNS_OVER,
    IND_MM_BTNS_PUSHED,

    // ICONS
    IND_MM_ICON_STEAM,
    IND_MM_ICON_DISCORD,
    IND_MM_ICON_TWITTER,
    IND_MM_ICON_YOUTUBE,
    IND_MM_ICON_LINKEDIN,

    NUM_MAIN_MENU_SPRITES
};

// MAP PANELS
extern const char SpriteFileMapPanels[];

enum SpriteIdMapPanels : unsigned int
{
    // MINIMAP
    IND_MINIMAP_BG,
    IND_MINIMAP_MAP_BG,
    IND_MINIMAP_MAP_ELEM,
    IND_MINIMAP_BTN_CLOSE_NORMAL,
    IND_MINIMAP_BTN_CLOSE_OVER,
    IND_MINIMAP_BTN_CLOSE_PUSHED,
    IND_MINIMAP_BTN_U_NORMAL,
    IND_MINIMAP_BTN_U_DISABLED,
    IND_MINIMAP_BTN_U_OVER,
    IND_MINIMAP_BTN_U_PUSHED,
    IND_MINIMAP_BTN_D_NORMAL,
    IND_MINIMAP_BTN_D_DISABLED,
    IND_MINIMAP_BTN_D_OVER,
    IND_MINIMAP_BTN_D_PUSHED,
    IND_MINIMAP_BTN_R_NORMAL,
    IND_MINIMAP_BTN_R_DISABLED,
    IND_MINIMAP_BTN_R_OVER,
    IND_MINIMAP_BTN_R_PUSHED,
    IND_MINIMAP_BTN_L_NORMAL,
    IND_MINIMAP_BTN_L_DISABLED,
    IND_MINIMAP_BTN_L_OVER,
    IND_MINIMAP_BTN_L_PUSHED,
    IND_MINIMAP_CAMERA_BL,
    IND_MINIMAP_CAMERA_TR,
    IND_MINIMAP_CAMERA_TL,
    IND_MINIMAP_CAMERA_BR,
    // MINIMAP BUTTON
    IND_MINIMAP_BUTTON_NORMAL,
    IND_MINIMAP_BUTTON_OVER,
    IND_MINIMAP_BUTTON_PUSHED
};

// MAP UI
extern const char SpriteFileMapUI[];

enum SpriteIdMapUI : unsigned int
{
    // CELL PROGRESS BAR
    IND_CELL_BAR_BG,
    IND_CELL_BAR_F1,
    IND_CELL_BAR_F2,
    IND_CELL_BAR_F3,
    IND_CELL_BAR_F4,
    // ICON COSTS
    IND_ICON_CELL_ENERGY,
    IND_ICON_CELL_MATERIAL
};

// PLANET MAP
extern const char SpriteFilePlanetMap[];

enum SpriteIdPlanetMap : unsigned int
{
    // PLANET
    IND_PM_PLANET,
    IND_PM_CELL_NORMAL,
    IND_PM_CELL_EXPLORED,
    IND_PM_CELL_SELECTED,
    IND_PM_CELL_DISABLED,
    IND_PM_CELL_F1,
    IND_PM_CELL_F1_SEL,
    IND_PM_CELL_F2,
    IND_PM_CELL_F2_SEL,
    IND_PM_CELL_F3,
    IND_PM_CELL_F3_SEL,

    // PANELS  TOP
    IND_PM_PANEL_NAME,
    IND_PM_PANEL_DATE,
};

// RESOURCES BAR
extern const char SpriteFileResourcesBar[];

enum SpriteIdResourcesBar : unsigned int
{
    // BAR
    IND_RESBAR_BG,

    // MINI BAR
    IND_RESBAR_MINIBAR_BG,
    IND_RESBAR_MINIBAR_VAL,

    // ICONS
    IND_RESBAR_MONEY,
    IND_RESBAR_ENERGY,
    IND_RESBAR_MATERIAL,
    IND_RESBAR_DIAMOND,
    IND_RESBAR_BLOB
};

// UNIT QUICK SELECTION
extern const char SpriteFileUnitQuickSel[];

enum SpriteIdUnitQuickSel : unsigned int
{
    // BUTTON BG
    IND_UQS_BG_NORMAL,
    IND_UQS_BG_DISABLED,
    IND_UQS_BG_MOUSE_OVER,
    IND_UQS_BG_PUSHED,
    IND_UQS_BG_CHECKED,

    // ICONS
    IND_UQS_ICON_ENERGY,
    IND_UQS_ICON_HEALTH,

    // UNITS
    IND_UQS_UNIT1_F1,
    IND_UQS_UNIT2_F1,
    IND_UQS_UNIT3_F1,
    IND_UQS_UNIT4_F1,
    IND_UQS_UNIT1_F2,
    IND_UQS_UNIT2_F2,
    IND_UQS_UNIT3_F2,
    IND_UQS_UNIT4_F2,
    IND_UQS_UNIT1_F3,
    IND_UQS_UNIT2_F3,
    IND_UQS_UNIT3_F3,
    IND_UQS_UNIT4_F3,

    NUM_UQS_UNIT_SPRITE_PER_FACTION = IND_UQS_UNIT1_F2 - IND_UQS_UNIT1_F1
};

} // namespace game
