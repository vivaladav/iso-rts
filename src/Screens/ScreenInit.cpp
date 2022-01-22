#include "Screens/ScreenInit.h"

#include "Game.h"
#include "GameData.h"
#include "States/StatesIds.h"
#include "Widgets/GameUIData.h"

#include <graphic/Font.h>
#include <graphic/FontManager.h>
#include <graphic/Image.h>
#include <graphic/Renderer.h>
#include <graphic/TextureManager.h>
#include <sgui/Label.h>
#include <sgui/Stage.h>

#include <cmath>
#include <sstream>

namespace game
{

ScreenInit::ScreenInit(Game * game)
    : Screen(game)
{
    game->SetClearColor(0x12, 0x12, 0x12, 0xFF);

    // -- BACKGROUND --
    mBg = new lib::graphic::Image("data/img/space_bg.jpg");

    // == SETUP JOBS ==
    auto tm = lib::graphic::TextureManager::Instance();

    // MAIN MENU
    mJobs.emplace_back([tm]
    {
        std::vector<lib::core::Rectd> rects
        {
            // PRIMARY BUTTON
            { 0, 0, 360, 70 },
            { 0, 70, 360, 70 },
            { 0, 140, 360, 70 },
            { 0, 210, 360, 70 },
            { 0, 280, 360, 70 },
            // WISHLIST BUTTON
            { 0, 350, 285, 50 },
            { 0, 400, 285, 50 },
            { 0, 450, 285, 50 },
            // SOCIAL BUTTON
            { 285, 350, 90, 50 },
            { 285, 400, 90, 50 },
            { 285, 450, 90, 50 },
            // ICONS
            { 0, 500, 32, 24 },
            { 32, 500, 32, 24 },
            { 64, 500, 30, 24 },
            { 94, 500, 34, 24 },
            { 128, 500, 24, 24 }
        };

        tm->RegisterSprite(SpriteFileMainMenu, rects);
    });

    // FACTION SELECTION
    mJobs.emplace_back([tm]
    {
        std::vector<lib::core::Rectd> rects
        {
            // PANELS
            { 0, 0, 1102, 970 },
            { 1103, 0, 300, 260 },
            { 1103, 261, 300, 260 },
            { 1103, 522, 300, 260 },
            { 1103, 783, 300, 110 },
            { 1404, 0, 300, 370 },
            // ATTRIBUTES BAR
            { 1103, 894, 187, 24 },
            { 1103, 918, 187, 24 },
            { 1103, 942, 187, 24 },
            { 1103, 966, 187, 24 },
            { 1103, 990, 187, 24 },
            { 1404, 819, 187, 24 },
            { 1404, 843, 187, 24 },
            { 1404, 867, 187, 24 },
            { 1404, 891, 187, 24 },
            { 1404, 915, 187, 24 },
            { 1404, 939, 187, 24 },
            // BACK BUTTON
            { 0, 971, 320, 50 },
            { 321, 971, 320, 50 },
            { 642, 971, 320, 50 },
            // SELECT BUTTON
            { 1404, 615, 250, 50 },
            { 1404, 666, 250, 50 },
            { 1404, 717, 250, 50 },
            { 1404, 768, 250, 50 },
            // CONTINUE BUTTON
            { 1404, 371, 300, 60 },
            { 1404, 432, 300, 60 },
            { 1404, 493, 300, 60 },
            { 1404, 554, 300, 60 }
        };

        tm->RegisterSprite(SpriteFileFactionSelection, rects);
    });

    // CELLS
    mJobs.emplace_back([tm]
    {
        const int indRows = 2;
        const int indCols = 9;
        const int indW = 96;
        const int indH = 48;
        int x = 0;
        int y = 0;

        std::vector<lib::core::Rectd> rects;
        rects.reserve(indRows * indCols);

        for(int r = 0; r < indRows; ++r)
        {
            for(int c = 0; c < indCols; ++c)
            {
                rects.emplace_back(x, y, indW, indH);

                x += indW;
            }

            x = 0;
            y += indH;
        }

        tm->RegisterSprite(SpriteFileCells, rects);
    });

    // MAP UI
    mJobs.emplace_back([tm]
    {
        const std::vector<lib::core::Rectd> rects
        {
            // cell progress bar
            { 0, 0, 52, 8 },
            { 0, 9, 50, 6 },
            { 0, 16, 50, 6 },
            { 0, 23, 50, 6 },
            { 0, 30, 50, 6 },
            // icons cost
            { 0, 37, 8, 14 },
            { 9, 37, 11, 12 }
        };

        tm->RegisterSprite(SpriteFileMapUI, rects);
    });

    // COLLECTIBLES
    mJobs.emplace_back([tm]
    {
        const std::vector<lib::core::Rectd> rects
        {
            // diamonds
            { 0, 0, 96, 51 },
            { 96, 0, 96, 51 },
            { 192, 0, 96, 51 },
            { 288, 0, 96, 70 },

            // blobs
            { 0, 70, 96, 48 },
            { 96, 70, 96, 48 },
            { 192, 70, 96, 48 },
            { 288, 70, 96, 58 }
        };

        tm->RegisterSprite(SpriteCollectiblesFile, rects);
    });

    // INDICATORS
    mJobs.emplace_back([tm]
    {
        const int indRows = 4;
        const int indCols = 6;
        const int indW = 96;
        const int indH = 48;
        int x = 0;
        int y = 0;

        std::vector<lib::core::Rectd> rects;
        rects.reserve(indRows * indCols);

        for(int r = 0; r < indRows; ++r)
        {
            for(int c = 0; c < indCols; ++c)
            {
                rects.emplace_back(x, y, indW, indH);

                if(rects.size() == NUM_IND_SPRITES)
                    break;

                x += indW;
            }

            if(rects.size() == NUM_IND_SPRITES)
                break;

            x = 0;
            y += indH;
        }

        tm->RegisterSprite(SpriteFileIndicators, rects);
    });

    // PARTICLES
    mJobs.emplace_back([tm]
    {
        const std::vector<lib::core::Rectd> rects
        {
            { 0, 0, 4, 4 }
        };

        tm->RegisterSprite(SpriteFileParticles, rects);
    });

    // ROCKS
    mJobs.emplace_back([tm]
    {
        const std::vector<lib::core::Rectd> rects
        {
            // row 0
            { 0, 0, 96, 51 },
            { 96, 0, 96, 54 },
            { 192, 0, 96, 54 },
            { 288, 0, 96, 51 },
            { 384, 0, 96, 56 },
            { 480, 0, 96, 55 },
            { 576, 0, 96, 58 },
            { 672, 0, 96, 56 },
            { 768, 0, 96, 55 },
            { 864, 0, 96, 58 },
            // row 1
            { 0, 58, 96, 60 },
            { 96, 58, 96, 57 },
            { 192, 58, 96, 53 },
            { 288, 58, 96, 60 },
            { 384, 58, 96, 59 },
            { 480, 58, 96, 59 },
            { 576, 58, 96, 59 },
            { 672, 58, 96, 59 },
            { 768, 58, 96, 59 }
        };

        tm->RegisterSprite(SpriteRocksFile, rects);
    });

    // STRUCTURES
    mJobs.emplace_back([tm]
    {
        const std::vector<lib::core::Rectd> rects
        {
            // base lvl 1
            { 0, 0, 288, 163 },
            { 288, 0, 288, 163 },
            { 576, 0, 288, 163 },
            { 0, 163, 288, 163 },
            { 288, 163, 288, 163 },
            { 576, 163, 288, 163 },
            // def tower lvl 1
            { 0, 326, 96, 65 },
            { 96, 326, 96, 65 },
            { 192, 326, 96, 65 },
            { 288, 326, 96, 65 },
            { 384, 326, 96, 65 },
            { 480, 326, 96, 65 },
            // def tower lvl 2
            { 0, 391, 96, 78 },
            { 96, 391, 96, 78 },
            { 192, 391, 96, 78 },
            { 288, 391, 96, 78 },
            { 384, 391, 96, 78 },
            { 480, 391, 96, 78 },
            // def tower lvl 3
            { 0, 469, 96, 84 },
            { 96, 469, 96, 84 },
            { 192, 469, 96, 84 },
            { 288, 469, 96, 84 },
            { 384, 469, 96, 84 },
            { 480, 469, 96, 84 },
            // practice target
            { 576, 343, 96, 48 },
            { 672, 343, 96, 48 },
            // radar
            { 0, 553, 192, 119 },
            { 192, 553, 192, 119 },
            { 384, 553, 192, 119 },
            { 576, 553, 192, 119 },
            { 0, 672, 192, 119 },
            { 192, 672, 192, 119 },
            { 384, 672, 192, 119 },
            // solar panel
            { 576, 791, 96, 55 },
            { 0, 791, 96, 55 },
            { 96, 791, 96, 55 },
            { 192, 791, 96, 55 },
            { 288, 791, 96, 55 },
            { 384, 791, 96, 55 },
            { 480, 791, 96, 55 },
            // energy generator
            { 0, 847, 192, 136 },
            { 192, 847, 192, 136 },
            { 384, 847, 192, 136 },
            { 576, 847, 192, 136 },
            { 0, 984, 192, 136 },
            { 192, 984, 192, 136 },
            { 384, 984, 192, 136 },
            // material generator
            { 0, 1121, 192, 134 },
            { 192, 1121, 192, 134 },
            { 384, 1121, 192, 134 },
            { 576, 1121, 192, 134 },
            { 0, 1256, 192, 134 },
            { 192, 1256, 192, 134 },
            { 384, 1256, 192, 134 },
            // storage energy
            { 866, 0, 96, 95 },
            { 769, 343, 96, 95 },
            { 769, 439, 96, 95 },
            { 769, 535, 96, 95 },
            { 769, 631, 96, 95 },
            { 769, 727, 96, 95 },
            { 769, 823, 96, 95 },
            // storage material
            { 866, 96, 96, 80 },
            { 769, 919, 96, 80 },
            { 769, 1000, 96, 80 },
            { 769, 1081, 96, 80 },
            { 769, 1162, 96, 80 },
            { 769, 1243, 96, 80 },
            { 769, 1324, 96, 80 },
            // storage diamonds
            { 866, 177, 96, 88 },
            { 866, 343, 96, 88 },
            { 866, 432, 96, 88 },
            { 866, 521, 96, 88 },
            { 866, 610, 96, 88 },
            { 866, 699, 96, 88 },
            { 866, 788, 96, 88 },
            // storage blobs
            { 866, 266, 96, 56 },
            { 866, 877, 96, 56 },
            { 866, 934, 96, 56 },
            { 866, 991, 96, 56 },
            { 866, 1048, 96, 56 },
            { 866, 1105, 96, 56 },
            { 866, 1162, 96, 56 }
        };

        tm->RegisterSprite(SpriteFileStructures, rects);
    });

    // UNITS
    mJobs.emplace_back([tm]
    {
        const std::vector<lib::core::Rectd> rects
        {
            // unit 1
            { 0, 0, 96, 58 },
            { 96, 0, 96, 58 },
            { 192, 0, 96, 58 },
            { 288, 0, 96, 58 },
            { 384, 0, 96, 58 },
            { 480, 0, 96, 58 },
            // unit 2
            { 0, 58, 96, 54 },
            { 96, 58, 96, 54 },
            { 192, 58, 96, 54 },
            { 288, 58, 96, 54 },
            { 384, 58, 96, 54 },
            { 480, 58, 96, 54 },
            // unit 3
            { 0, 112, 96, 54 },
            { 96, 112, 96, 54 },
            { 192, 112, 96, 54 },
            { 288, 112, 96, 54 },
            { 384, 112, 96, 54 },
            { 480, 112, 96, 54 }
        };

        tm->RegisterSprite(SpriteFileUnits, rects);
    });

    // UNIT PARTICLES
    mJobs.emplace_back([tm]
    {
        const std::vector<lib::core::Rectd> rectsUnitsPart
        {
            { 0, 0, 10, 2 },
            { 0, 2, 10, 2 },
            { 0, 4, 10, 2 }
        };

        tm->RegisterSprite(SpriteFileUnitsParticles, rectsUnitsPart);
    });

    // WALLS
    mJobs.emplace_back([tm]
    {
        // level 1 walls
        const int indRows = 3;
        const int indCols = 11;
        const int indW = 96;
        const int indH = 48;
        int x = 0;
        int y = 0;

        std::vector<lib::core::Rectd> rects;
        rects.reserve(indRows * indCols);

        for(int r = 0; r < indRows; ++r)
        {
            for(int c = 0; c < indCols; ++c)
            {
                rects.emplace_back(x, y, indW, indH);
                x += indW;
            }

            x = 0;
            y += indH;
        }

        tm->RegisterSprite(SpriteWallsFile, rects);
    });

    // NEW ELEMENT DIALOG
    mJobs.emplace_back([tm]
    {
        const std::vector<lib::core::Rectd> rects
        {
            // BACKGROUND
            { 0, 0, 1280, 610 },
            // PANELS
            { 0, 611, 400, 38 },
            { 401, 611, 400, 38 },
            { 804, 611, 400, 260 },
            { 0, 651, 200, 200 },
            { 201, 651, 200, 200 },
            { 402, 651, 200, 200 },
            { 603, 651, 200, 200 },
            // ATTRIBUTES BAR
            { 0, 894, 140, 18 },
            { 141, 894, 140, 18 },
            { 282, 894, 140, 18 },
            { 423, 894, 140, 18 },
            { 564, 894, 140, 18 },
            { 705, 894, 140, 18 },
            { 0, 913, 140, 18 },
            { 141, 913, 140, 18 },
            { 282, 913, 140, 18 },
            { 423, 913, 140, 18 },
            { 564, 913, 140, 18 },
            // BUTTON CLOSE
            { 846, 872, 100, 40 },
            { 947, 872, 100, 40 },
            { 1048, 872, 100, 40 },
            // BUTTON BUILD
            { 0, 853, 200, 40 },
            { 201, 853, 200, 40 },
            { 402, 853, 200, 40 },
            { 603, 853, 200, 40 },
            // ICONS
            { 1205, 611, 16, 16 },
            { 1222, 611, 16, 16 },
            { 1239, 611, 16, 16 },
            { 1256, 611, 16, 16 },
            // BUTTON LEFT
            { 1281, 0, 20, 80 },
            { 1281, 81, 20, 80 },
            { 1281, 162, 20, 80 },
            { 1281, 243, 20, 80 },
            // BUTTON RIGHT
            { 1281, 324, 20, 80 },
            { 1281, 405, 20, 80 },
            { 1281, 486, 20, 80 },
            { 1281, 567, 20, 80 }
        };

        tm->RegisterSprite(SpriteFileNewElementDialog, rects);
    });

    // OBJECT ACTION BUTTON
    mJobs.emplace_back([tm]
    {
        std::vector<lib::core::Rectd> rects;

        // button bg
        int indCols = 5;
        int indW = 64;
        int indH = 64;
        int x = 0;
        int y = 0;

        for(int c = 0; c < indCols; ++c)
        {
            rects.emplace_back(x, y, indW, indH);
            x += indW;
        }

        y += indH;

        // icons
        indCols = 8;
        indW = 48;
        indH = 48;

        x = 0;

        for(int c = 0; c < indCols; ++c)
        {
            rects.emplace_back(x, y, indW, indH);
            x += indW;
        }

        tm->RegisterSprite(SpriteFileObjActionButton, rects);
    });

    // RESOURCES BAR
    mJobs.emplace_back([tm]
    {
        const std::vector<lib::core::Rectd> rects
        {
            // BAR
            { 0, 0, 822, 50 },
            // MINI BAR
            { 0, 51, 54, 8 },
            { 0, 60, 50, 4 },
            // ICONS
            { 55, 51, 22, 32 },
            { 78, 51, 18, 32 },
            { 97, 51, 24, 26 },
            { 122, 51, 18, 30 },
            { 141, 51, 26, 26 }
        };

        tm->RegisterSprite(SpriteFileResourcesBar, rects);
    });


    // FINAL JOB - move to next screen
    // NOTE keep last
    mJobs.emplace_back([this]
    {
        GetGame()->RequestNextActiveState(StateId::MAIN_MENU);
    });

    // INIT STATUS LABEL
    auto fm = lib::graphic::FontManager::Instance();
    lib::graphic::Font * font = fm->GetFont("data/fonts/Lato-Regular.ttf", 32, lib::graphic::Font::NORMAL);

    mLabelStatus = new lib::sgui::Label(font);
    mLabelStatus->SetColor(0xEEEEEEFF);
    UpdateStatus();
}

ScreenInit::~ScreenInit()
{
    delete mBg;

    lib::sgui::Stage::Instance()->ClearWidgets();
}

void ScreenInit::Update(float update)
{
    mJobs[mCurrJob]();

    ++mCurrJob;

    UpdateStatus();
}

void ScreenInit::Render()
{
    mBg->Render();
}

void ScreenInit::UpdateStatus()
{
    // update status string
    const int perc = static_cast<int>(std::roundf(mCurrJob * 100.f / mJobs.size()));

    std::ostringstream ss;
    ss << "LOADING... " << perc << "%";
    mLabelStatus->SetText(ss.str().c_str());

    // update position
    const int rendW = lib::graphic::Renderer::Instance()->GetWidth();
    const int rendH = lib::graphic::Renderer::Instance()->GetHeight();
    const int x = (rendW - mLabelStatus->GetWidth()) * 0.5f;
    const int y = rendH - (mLabelStatus->GetHeight() * 2.f);
    mLabelStatus->SetPosition(x, y);
}

} // namespace game
