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
