#pragma once

namespace game
{

// COLLECTIBLES
extern const char SpriteCollectiblesFile[];

enum SpriteCollectiblesId : unsigned int
{
    DIAMONDS_1,
    DIAMONDS_2,
    DIAMONDS_3,
    DIAMONDS_4,

    BLOBS_1,
    BLOBS_2,
    BLOBS_3,
    BLOBS_4
};

// INDICATORS
extern const char SpriteFileIndicators[];

enum SpriteIdIndicators : unsigned int
{
    // WALL INDICATORS
    IND_WB_HORIZONTAL_F1,
    IND_WB_VERTICAL_F1,
    IND_WB_TOP_LEFT_F1,
    IND_WB_TOP_RIGHT_F1,
    IND_WB_BOTTOM_LEFT_F1,
    IND_WB_BOTTOM_RIGHT_F1,

    IND_WB_HORIZONTAL_F2,
    IND_WB_VERTICAL_F2,
    IND_WB_TOP_LEFT_F2,
    IND_WB_TOP_RIGHT_F2,
    IND_WB_BOTTOM_LEFT_F2,
    IND_WB_BOTTOM_RIGHT_F2,

    IND_WB_HORIZONTAL_F3,
    IND_WB_VERTICAL_F3,
    IND_WB_TOP_LEFT_F3,
    IND_WB_TOP_RIGHT_F3,
    IND_WB_BOTTOM_LEFT_F3,
    IND_WB_BOTTOM_RIGHT_F3,

    IND_WB_INVALID,

    // MOVE INDICATOR
    IND_MOVE,

    // CONQUEST INDICATOR
    IND_CONQUEST_F1,
    IND_CONQUEST_F2,
    IND_CONQUEST_F3,

    // TOTAL
    NUM_IND_SPRITES,

    // SPECIAL VALUES
    IND_WB_FIRST = IND_WB_HORIZONTAL_F1
};

// ROCKS
extern const char SpriteRocksFile[];

enum SpriteRocksId : unsigned int
{
    ROCKS_ROW_END_L_1,
    ROCKS_ROW_END_R_1,
    ROCKS_COL_END_L_1,
    ROCKS_COL_END_R_1,
    ROCKS_ROW_1,
    ROCKS_ROW_2,
    ROCKS_ROW_3,
    ROCKS_COL_1,
    ROCKS_COL_2,
    ROCKS_COL_3,
    ROCKS_CORN_BL_1,
    ROCKS_CORN_BR_1,
    ROCKS_CORN_TL_1,
    ROCKS_CORN_TR_1,
    ROCKS_INTN,
    ROCKS_INTS,
    ROCKS_INTW,
    ROCKS_INTE,
    ROCKS_CROSS
};

// WALLS
extern const char SpriteWallsFile[];

enum SpriteWallsId : unsigned int
{
    // wall level 1 faction 1
    WALL_L1_F1_HORIZ,
    WALL_L1_F1_VERT,
    WALL_L1_F1_TL,
    WALL_L1_F1_TR,
    WALL_L1_F1_BL,
    WALL_L1_F1_BR,
    WALL_L1_F1_INTN,
    WALL_L1_F1_INTS,
    WALL_L1_F1_INTW,
    WALL_L1_F1_INTE,
    WALL_L1_F1_CROSS,

    // wall level 1 faction 2
    WALL_L1_F2_HORIZ,
    WALL_L1_F2_VERT,
    WALL_L1_F2_TL,
    WALL_L1_F2_TR,
    WALL_L1_F2_BL,
    WALL_L1_F2_BR,
    WALL_L1_F2_INTN,
    WALL_L1_F2_INTS,
    WALL_L1_F2_INTW,
    WALL_L1_F2_INTE,
    WALL_L1_F2_CROSS,

    // wall level 1 faction 3
    WALL_L1_F3_HORIZ,
    WALL_L1_F3_VERT,
    WALL_L1_F3_TL,
    WALL_L1_F3_TR,
    WALL_L1_F3_BL,
    WALL_L1_F3_BR,
    WALL_L1_F3_INTN,
    WALL_L1_F3_INTS,
    WALL_L1_F3_INTW,
    WALL_L1_F3_INTE,
    WALL_L1_F3_CROSS
};

} // namespace game
