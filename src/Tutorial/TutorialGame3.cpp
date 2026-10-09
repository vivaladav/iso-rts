#include "Tutorial/TutorialGame3.h"

#include "CameraMapController.h"
#include "Game.h"
#include "GameMap.h"
#include "Player.h"
#include "AI/PlayerAI.h"
#include "GameObjects/Base.h"
#include "GameObjects/CityBlock.h"
#include "GameObjects/CityGroup.h"
#include "GameObjects/LootBox.h"
#include "GameObjects/MiniUnitsGroup.h"
#include "GameObjects/ObjectData.h"
#include "GameObjects/ObjectsDataRegistry.h"
#include "GameObjects/Unit.h"
#include "Indicators/OverlayCellConquest.h"
#include "Indicators/OverlayWall.h"
#include "Screens/ScreenGame.h"
#include "Tutorial/StepAISetActive.h"
#include "Tutorial/StepDelay.h"
#include "Tutorial/StepGameAddEnemy.h"
#include "Tutorial/StepGameBackToBase.h"
#include "Tutorial/StepGameBuildStructAreaFree.h"
#include "Tutorial/StepGameBuildStructIntro.h"
#include "Tutorial/StepGameBuildStructure.h"
#include "Tutorial/StepGameBuildTowerEnd.h"
#include "Tutorial/StepGameBuildUnitEnd.h"
#include "Tutorial/StepGameBuildUnitStart.h"
#include "Tutorial/StepGameBuildWallLine.h"
#include "Tutorial/StepGameCityIntro.h"
#include "Tutorial/StepGameClearSelection.h"
#include "Tutorial/StepGameClickActionButton.h"
#include "Tutorial/StepGameConnectStructFree.h"
#include "Tutorial/StepGameConnectStructIntro.h"
#include "Tutorial/StepGameConnectWall.h"
#include "Tutorial/StepGameConquerCellsContinue.h"
#include "Tutorial/StepGameConquerCellsEnd.h"
#include "Tutorial/StepGameConquerCellsSimple.h"
#include "Tutorial/StepGameConquerCity.h"
#include "Tutorial/StepGameConquerEnergyGenIntro.h"
#include "Tutorial/StepGameConquerStructFree.h"
#include "Tutorial/StepGameConquerMaterialGenIntro.h"
#include "Tutorial/StepGameConquerStructSimple.h"
#include "Tutorial/StepGameDialogTrading.h"
#include "Tutorial/StepGameDialogTradingBuy.h"
#include "Tutorial/StepGameDisableCamera.h"
#include "Tutorial/StepGameEndTurnSimple.h"
#include "Tutorial/StepGameEnemyIntro.h"
#include "Tutorial/StepGameEnemyKilled.h"
#include "Tutorial/StepGameEnding.h"
#include "Tutorial/StepGameIntro3.h"
#include "Tutorial/StepGameMissionGoalsIcon.h"
#include "Tutorial/StepGameMoveCamera.h"
#include "Tutorial/StepGameMoveCameraOverCell.h"
#include "Tutorial/StepGameMoveCameraOverObject.h"
#include "Tutorial/StepGameMoveToCellFree.h"
#include "Tutorial/StepGameMoveUnitFree.h"
#include "Tutorial/StepGameMoveUnitSimple.h"
#include "Tutorial/StepGameMoveUnitToCorner.h"
#include "Tutorial/StepGameOpenLootbox.h"
#include "Tutorial/StepGameOpenLootboxFree.h"
#include "Tutorial/StepGameQuickUnitButton.h"
#include "Tutorial/StepGameSecondaryMissionGoal.h"
#include "Tutorial/StepGameSelectObject.h"
#include "Tutorial/StepGameSetLootboxPrize.h"
#include "Tutorial/StepGameSetSelectionActiveAction.h"
#include "Tutorial/StepGameSetTarget.h"
#include "Tutorial/StepGameSetTargetIcon.h"
#include "Tutorial/StepGameSetupMiniUnits.h"
#include "Tutorial/StepGameSetupResearch.h"
#include "Tutorial/StepGameSetupResearchIcon.h"
#include "Tutorial/StepGameSingleInfo.h"
#include "Tutorial/StepGameSpawnIcon.h"
#include "Tutorial/StepGameTechTreeDialog.h"
#include "Tutorial/StepGameTechTreeIcon.h"
#include "Tutorial/StepGameTradeIcon.h"
#include "Tutorial/StepGameUnitConquerCellsIcon.h"
#include "Tutorial/StepGameUnit.h"
#include "Tutorial/StepGameUpgradeIntro.h"
#include "Tutorial/StepGameUpgradeUnit.h"
#include "Tutorial/StepGameUpgradeUnitFree.h"
#include "Tutorial/StepGameWaitEnemyKilled.h"
#include "Tutorial/StepGameWaitLootboxOpen.h"
#include "Tutorial/StepGameWaitObjectIdle.h"
#include "Tutorial/StepGameWaitTurn.h"
#include "Tutorial/StepGameWaitUnitCanBuild.h"
#include "Tutorial/StepGameWallBuildIcon.h"
#include "Tutorial/StepGameWallBuildIntro.h"
#include "Tutorial/StepGameWallBuildEnd.h"
#include "Tutorial/StepGameWallBuildStart.h"
#include "Tutorial/StepSaveGame.h"
#include "Tutorial/TutorialConstants.h"
#include "Widgets/GameHUD.h"
#include "Widgets/PanelObjectActions.h"

namespace
{
using namespace game;

constexpr float delayAnimLong = 2.f;

constexpr float speedCamera = 700.f;

const Cell2D cellTLCity1(51, 51);
const Cell2D cellBRCity1(54, 54);
const Cell2D cellCityBlock2(54, 52);
const Cell2D cellCityBlock3(52, 52);
const Cell2D cellCityBlock4(52, 54);
const Cell2D cellLootbox1(74, 63);
const Cell2D cellRadar(41, 41);
const Cell2D cellResCenter(45, 47);
const Cell2D cellSpecialLootbox1(61, 73);
const Cell2D cellMatGen1(66, 62);
const Cell2D cellMatGen2(41, 49);
const Cell2D cellMatGen3(51, 72);
const Cell2D cellMatGen4(81, 36);
const Cell2D cellEneGen1(62, 66);
const Cell2D cellEneGen2(49, 41);
const Cell2D cellEneGen3(73, 53);
const Cell2D cellEneGen4(40, 81);
const Cell2D cellTower1(58, 58);
const Cell2D cellTower2(48, 48);
const Cell2D cellTower3(41, 58);
const Cell2D cellTower4(56, 42);
const Cell2D cellTower5(42, 65);
const Cell2D cellTower6(64, 43);
const Cell2D cellTower7(70, 46);
const Cell2D cellTower8(45, 71);
const Cell2D cellTower9(79, 47);
const Cell2D cellTower10(47, 79);
const Cell2D cellBunker1(37, 50);
const Cell2D cellBunker2(50, 37);
const Cell2D cellBarracks(53, 47);
const Cell2D cellEnemy1(64,53);
const Cell2D cellMiniUnits1(71, 62);
const Cell2D cellTradingPost(70, 57);

const Cell2D wallLeft1TL(50, 37);
const Cell2D wallLeft1BR(54, 37);
const Cell2D wallLeft2TL(57, 37);
const Cell2D wallLeft2BR(62, 37);
const Cell2D wallRight1TL(37, 50);
const Cell2D wallRight1BR(37, 54);
const Cell2D wallRight2TL(37, 57);
const Cell2D wallRight2BR(37, 63);

}

namespace game
{

TutorialGame3::TutorialGame3(Screen * screen)
    : TutorialGame(screen, TUTORIAL_MISSION_3)
{
    using namespace sgl;

    auto gs = GetScreen();
    auto game = gs->GetGame();
    auto gameMap = GetGameMap();
    auto hud = gs->GetHUD();
    auto isoMap = GetIsoMap();

    const Player * local = game->GetActivePlayerByIndex(0);
    Player * playerAI = game->GetActivePlayerByIndex(1);

    auto panelActions = hud->GetPanelObjectActions();
    auto panelTurn = hud->GetPanelTurnControl();
    auto localBase = local->GetBase();

    // keep camera enabled in this tutorial
    GetCameraMapController()->SetEnabled(true);

    // ===== SETUP =====
    // pause before start
    AddStep([] { return new StepDelay(0.5f); });

    // ===== PART 1 =====
    // INTRO
    AddStep([] { return new StepGameIntro3; });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD WORKER 1
    AddStep([localBase, game, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        return new StepGameSelectObject(game, isoMap, localBase, "TUT_GAME_BUILD_WORKER", p0,
                                        delayAnimLong);
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_WORKER",
                                                  PanelObjectActions::BTN_BUILD_UNIT_BASE,
                                                  delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildUnitEnd(hud, "TUT_GAME_BUILD_WORKER", delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([localBase] { return new StepDelay(localBase->GetTimeBuildUnit()); });
    // INTRO BUILD SOLAR PANELS AND MATERIAL EXTRACTOR
    AddStep([]
    {
        const int h = 175;
        const core::Pointd2D p0(1200, 250);
        return new StepGameSingleInfo(p0, "TUT_GAME_BUILD_RES_GEN", h);
    });
    // BUILD SOLAR PANELS
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 100);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_RES_GEN_ENERGY_SOLAR);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_SOL_PANELS");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_SOL_PANELS",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud, "TUT_GAME_CAT_RES_GEN", nullptr,
                                               "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catResources,
                                               TutorialConstants::structSolarPanel);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const Cell2D areaTL(72, 72);
        const Cell2D areaBR(75, 80);
        const int numToBuild = 2;

        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, areaTL, areaBR,
                                                    ObjectData::TYPE_RES_GEN_ENERGY_SOLAR, numToBuild,
                                                    "TUT_GAME_BUILD_SOL_PANELS");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // CONNECT SECOND SOLAR PANEL IF NEEDED
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const auto all = local->GetStructuresByType(ObjectData::TYPE_RES_GEN_ENERGY_SOLAR);
        const GameObject * barracks = all.back();
        const core::Pointd2D p0(1200, 400);
        return new StepGameConnectStructFree(game, barracks, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // CONNECT FIRST SOLAR PANEL IF NEEDED
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const auto all = local->GetStructuresByType(ObjectData::TYPE_RES_GEN_ENERGY_SOLAR);
        const auto it = all.end() - 2;
        const GameObject * barracks = *it;
        const core::Pointd2D p0(1200, 400);
        return new StepGameConnectStructFree(game, barracks, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD MATERIAL EXTRACTOR
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 100);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_RES_GEN_MATERIAL_EXTRACT);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_MAT_EXTR_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_MAT_EXTR_1",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud, "TUT_GAME_CAT_RES_GEN",
                                               "TUT_GAME_BUILD_MAT_EXTR_2",
                                               "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catResources,
                                               TutorialConstants::structMatExtr);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const Cell2D areaTL(72, 70);
        const Cell2D areaBR(75, 78);
        const int numToBuild = 1;

        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, areaTL, areaBR,
                                                    ObjectData::TYPE_RES_GEN_MATERIAL_EXTRACT,
                                                    numToBuild, "TUT_GAME_BUILD_MAT_EXTR_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // CONNECT MATERIAL EXTRACTOR IF NEEDED
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const auto all = local->GetStructuresByType(ObjectData::TYPE_RES_GEN_MATERIAL_EXTRACT);
        const GameObject * barracks = all.back();
        const core::Pointd2D p0(1200, 400);
        return new StepGameConnectStructFree(game, barracks, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // INTRO MOVE WORKER TO LEFT SIDE
    AddStep([]
    {
        const core::Pointd2D p0(1300, 200);
        return new StepGameSingleInfo(p0, "TUT_GAME_MOVE_UNIT_3");
    });
    // MOVE CAMERA OVER TARGET
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([isoMap]
    {
        const Cell2D cell(74, 68);
        return new StepGameMoveCameraOverCell(cell, isoMap, speedCamera);
    });
    // MOVE WORKER TO LEFT SIDE
    AddStep([game, isoMap, local]
    {
        const core::Pointd2D p0(1300, 200);
        const Cell2D target(74, 66);
        const auto all = local->GetUnitsByType(ObjectData::TYPE_UNIT_WORKER1);
        const auto unit = all.back();
        return new StepGameMoveUnitFree(game, unit, isoMap, "TUT_GAME_MOVE_UNIT",
                                        target, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // SETUP LOOTBOX
    AddStep([this]
    {
        auto lootbox = static_cast<LootBox *>(GetObjectInCell(cellLootbox1));
        const unsigned int prizeType = LootBox::LB_MATERIAL;
        const int prizeQuantity = 350;
        return new StepGameSetLootboxPrize(lootbox, prizeType, prizeQuantity);
    });
    // OPEN LOOTBOX
    AddStep([this, local, game, isoMap]
    {
        const auto all = local->GetUnitsByType(ObjectData::TYPE_UNIT_WORKER1);
        const auto unit = all.back();
        const GameObject * lootbox = GetObjectInCell(cellLootbox1);
        const core::Pointd2D p0(500, 100);

        return new StepGameOpenLootbox(game, unit, lootbox, isoMap, "TUT_GAME_LOOTBOX_1", p0);
    });
    AddStep([this]
    {
        const GameObject * lootbox = GetObjectInCell(cellLootbox1);
        return new StepGameWaitLootboxOpen(GetGameMap(), lootbox);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // CONTINUE MOVE
    AddStep([]
    {
        const core::Pointd2D p0(1200, 250);
        return new StepGameSingleInfo(p0, "TUT_GAME_CONT_EXPL");
    });
    // MOVE WORKER TO MATERIAL GENERATOR
    AddStep([game, isoMap, local]
    {
        const core::Pointd2D p0(1300, 200);
        const Cell2D target(68, 62);
        const auto all = local->GetUnitsByType(ObjectData::TYPE_UNIT_WORKER1);
        const auto unit = all.back();
        return new StepGameMoveUnitFree(game, unit, isoMap, "TUT_GAME_MOVE_UNIT",
                                        target, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER MATERIAL GENERATOR 1
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen1);
        const core::Pointd2D p0(1300, 300);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_MAT_GEN", p0);
    });
    // MOVE VIEW BACK TO BASE
    AddStep([panelTurn, game]
        {
            const core::Pointd2D p0(50, 600);
            return new StepGameBackToBase(panelTurn, "TUT_GAME_BACK_TO_BASE_1", p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD WORKER UNIT 2
    AddStep([localBase, game, isoMap]
        {
            const core::Pointd2D p0(500, 200);
            return new StepGameSelectObject(game, isoMap, localBase, "TUT_GAME_BASE_4",
                                            p0, delayAnimLong);
        });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_WORKER",
                                                  PanelObjectActions::BTN_BUILD_UNIT_BASE,
                                                  delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildUnitEnd(hud, "TUT_GAME_BUILD_WORKER", delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([localBase] { return new StepDelay(localBase->GetTimeBuildUnit()); });
    // MOVE CAMERA
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([game] { return new StepGameMoveCamera(0, -350); });
    // CONNECT MATERIAL GENERATOR 1
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen1);
        const core::Pointd2D p0(1300, 400);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA NEAR ENERGY GENERATOR 1
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([isoMap]
    {
        const Cell2D cell(64, 65);
        return new StepGameMoveCameraOverCell(cell, isoMap, speedCamera);
    });
    // CONQUER ENERGY GENERATOR 1
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen1);
        const core::Pointd2D p0(1300, 200);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_ENE_GEN", p0);
    });
    // CONNECT ENERGY GENERATOR 1
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen1);
        const core::Pointd2D p0(1300, 300);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // SETUP SPECIAL LOOTBOX
    AddStep([this]
    {
        auto lootbox = static_cast<LootBox *>(GetObjectInCell(cellSpecialLootbox1));
        const unsigned int prizeType = LootBox::LB_NULL;
        const int prizeQuantity = 0;
        return new StepGameSetLootboxPrize(lootbox, prizeType, prizeQuantity);
    });
    // OPEN SPECIAL LOOTBOX
    AddStep([this, gameMap, isoMap]
    {
        const core::Pointd2D p0(1000, 200);
        auto lootbox = static_cast<LootBox *>(GetObjectInCell(cellSpecialLootbox1));
        return new StepGameOpenLootboxFree(gameMap, lootbox, isoMap, "TUT_GAME_LOOTBOX_1b", p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([]
    {
        const core::Pointd2D p0(1150, 350);
        return new StepGameSingleInfo(p0, "TUT_GAME_NO_LUCK");
    });
    AddStep([] { return new StepDelay(0.5f); });
    // INTRO MOVE WORKER TO LEFT SIDE
    AddStep([]
    {
        const core::Pointd2D p0(1300, 200);
        return new StepGameSingleInfo(p0, "TUT_GAME_MOVE_UNIT_3");
    });
    // MOVE CAMERA OVER TARGET
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([isoMap]
    {
        const Cell2D cell(59, 59);
        return new StepGameMoveCameraOverCell(cell, isoMap, speedCamera);
    });
    // MOVE WORKER TO TOP
    AddStep([gameMap, isoMap, local]
    {
        const core::Pointd2D p0(1300, 200);
        const Cell2D target(56, 56);
        return new StepGameMoveToCellFree(gameMap, isoMap, "TUT_GAME_MOVE_UNIT_6",
                                          target, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // INTRODUCE FIRST CITY AREA
    AddStep([isoMap]
    {
        const core::Pointd2D p0(1300, 150);
        return new StepGameCityIntro(cellTLCity1, cellBRCity1, isoMap, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER CITY
    AddStep([this, local, isoMap, game]
    {
        const auto cb = static_cast<const CityBlock *>(GetObjectInCell(cellBRCity1));
        const auto cg = static_cast<const CityGroup *>(cb->GetGroup());
        const core::Pointd2D p0(1300, 200);
        return new StepGameConquerCity(cg, isoMap, "TUT_GAME_CITY_2", p0);
    });
    // CONNECT CITY
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellBRCity1);
        const core::Pointd2D p0(1300, 300);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // INTRO MOVE WORKER TO LEFT SIDE
    AddStep([]
    {
        const core::Pointd2D p0(1300, 200);
        return new StepGameSingleInfo(p0, "TUT_GAME_CONT_EXPL");
    });
    // MOVE CAMERA OVER TARGET
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([isoMap]
    {
        const Cell2D cell(52, 52);
        return new StepGameMoveCameraOverCell(cell, isoMap, speedCamera);
    });
    // MOVE WORKER TO TOP
    AddStep([gameMap, isoMap, local]
    {
        const core::Pointd2D p0(1300, 200);
        const Cell2D target(47, 47);
        return new StepGameMoveToCellFree(gameMap, isoMap, "TUT_GAME_MOVE_UNIT_6",
                                          target, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER MATERIAL GENERATOR 2
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen2);
        const core::Pointd2D p0(1300, 150);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_MAT_GEN", p0);
    });
    // CONNECT MATERIAL GENERATOR 2
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen2);
        const core::Pointd2D p0(1300, 500);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER ENERGY GENERATOR 2
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen2);
        const core::Pointd2D p0(1300, 200);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_ENE_GEN", p0);
    });
    // CONNECT ENERGY GENERATOR 2
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen2);
        const core::Pointd2D p0(1300, 200);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD RESEARCH CENTER
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 100);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_RESEARCH_CENTER);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_RES_CEN_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_RES_CEN_1",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud, "TUT_GAME_BUILD_RES_CEN_2", nullptr,
                                               "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catTech,
                                               TutorialConstants::structResearchCenter);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const Cell2D areaTL(42, 42);
        const Cell2D areaBR(47, 47);
        const int numToBuild = 1;

        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, areaTL, areaBR,
                                                    ObjectData::TYPE_RESEARCH_CENTER, numToBuild,
                                                    "TUT_GAME_BUILD_RES_CEN_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // CONNECT RESEARCH CENTER IF NEEDED
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const auto all = local->GetStructuresByType(ObjectData::TYPE_RESEARCH_CENTER);
        const GameObject * rc = all.back();
        const core::Pointd2D p0(1300, 400);
        return new StepGameConnectStructFree(game, rc, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // SELECT RESEARCH CENTER
    AddStep([local, game, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const auto all = local->GetStructuresByType(ObjectData::TYPE_RESEARCH_CENTER);
        const GameObject * rc = all.back();

        return new StepGameSelectObject(game, isoMap, rc, "TUT_GAME_RES_CEN_1", p0,
                                        delayAnimLong);
    });
    // SETUP RESEARCH
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1000, 650);
        return new StepGameSetupResearchIcon(game, gs, panelActions, p0);
    });
    AddStep([hud]
    {
        return new StepGameSetupResearch(hud, { 100, 80, 70 });
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this]
    {
        const GameObject * obj = GetObjectInCell(cellEneGen2);
        const float speed = 800.f;
        return new StepGameMoveCameraOverObject(obj, speed);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD WALL LEFT 1
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1200, 200);
        auto step = new StepGameBuildWallLine(gameMap, isoMap, wallLeft1TL, wallLeft1BR,
                                              "TUT_GAME_BUILD_WALL_5");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // CONNECT WALL LEFT 1
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 200);
        auto step = new StepGameConnectWall(gameMap, isoMap, wallLeft1TL, wallLeft1BR,
                                            "TUT_GAME_CONNECT_WALL");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD BUNKER IN LEFT WALL 1
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 600);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_BUNKER);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_BUNKER_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_BUNKER_1",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud,"TUT_GAME_BUILD_DTOWER_3", nullptr,
                                               "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catDefenses,
                                               TutorialConstants::structBunker);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const unsigned int numToBuild = 1;
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallLeft1TL, wallLeft1BR,
                                                    ObjectData::TYPE_BUNKER, numToBuild,
                                                    "TUT_GAME_BUILD_BUNKER_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD WALL LEFT 2
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1200, 200);
        auto step = new StepGameBuildWallLine(gameMap, isoMap, wallLeft2TL, wallLeft2BR,
                                              "TUT_GAME_BUILD_WALL_5");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // CONNECT WALL LEFT 2
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 200);
        auto step = new StepGameConnectWall(gameMap, isoMap, wallLeft2TL, wallLeft2BR,
                                            "TUT_GAME_CONNECT_WALL");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // SELECT RESEARCH CENTER AND UNLOCK UPGRADE
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local]
    {
        const auto all = local->GetStructuresByType(ObjectData::TYPE_RESEARCH_CENTER);
        const GameObject * rc = all.back();
        const float speed = 1000.f;
        return new StepGameMoveCameraOverObject(rc, speed);
    });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([]
    {
        const core::Pointd2D p0(685, 200);
        return new StepGameSingleInfo(p0, "TUT_GAME_RES_CEN_2");
    });
    AddStep([this, game, isoMap, local]
    {
        const core::Pointd2D p0(710, 250);
        const auto all = local->GetStructuresByType(ObjectData::TYPE_RESEARCH_CENTER);
        const GameObject * rc = all.back();
        return new StepGameSelectObject(game, isoMap, rc, "TUT_GAME_RES_CEN_1", p0);
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1000, 650);
        return new StepGameTechTreeIcon(game, gs, panelActions, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([hud] { return new StepGameTechTreeDialog(hud, TECH_UP_RADAR_STATION, false); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA BACK TO WALL LEFT
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([isoMap]
    {
        return new StepGameMoveCameraOverCell(wallLeft2TL, isoMap, speedCamera);
    });
    // BUILD GATE IN LEFT WALL 2
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 100);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_WALL_GATE);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_GATE_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_GATE_1",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud,"TUT_GAME_BUILD_DTOWER_3",
                                               "TUT_GAME_BUILD_GATE_2",
                                               "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catDefenses,
                                               TutorialConstants::structGate);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const unsigned int numToBuild = 1;
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallLeft2TL, wallLeft2BR,
                                                    ObjectData::TYPE_WALL_GATE, numToBuild,
                                                    "TUT_GAME_BUILD_GATE_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([] { return new StepDelay(0.5f); });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD TOWER IN LEFT WALL 2
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 600);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_DEFENSIVE_TOWER);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_DTOWER_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_DTOWER_1",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud,"TUT_GAME_BUILD_DTOWER_3",
                                               "TUT_GAME_BUILD_GATE_2",
                                               "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catDefenses,
                                               TutorialConstants::structDefTower);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const unsigned int numToBuild = 1;
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallLeft2TL, wallLeft2BR,
                                                    ObjectData::TYPE_DEFENSIVE_TOWER, numToBuild,
                                                    "TUT_GAME_BUILD_DTOWER_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE VIEW BACK TO BASE
    AddStep([panelTurn, game]
    {
        const core::Pointd2D p0(50, 600);
        return new StepGameBackToBase(panelTurn, "TUT_GAME_BACK_TO_BASE_1", p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD SPAWNER
    AddStep([localBase, game, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        return new StepGameSelectObject(game, isoMap, localBase, "TUT_GAME_BASE_4", p0,
                                        delayAnimLong);
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_SPAWNER_1",
                                                  PanelObjectActions::BTN_BUILD_UNIT_BASE,
                                                  delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        return new StepGameBuildStructure(hud, nullptr, "TUT_GAME_BUILD_SPAWNER_2",
                                          "TUT_GAME_BASE_BUILD_UNIT_3",
                                          TutorialConstants::catUnitGeneric,
                                          TutorialConstants::unitMiniSpawner);
    });
    AddStep([localBase] { return new StepDelay(localBase->GetTimeBuildUnit()); });
    // SELECT SPAWNER
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([local, game, isoMap]
    {
        const auto all = local->GetUnitsByType(ObjectData::TYPE_UNIT_SPAWNER1);
        const auto unit = all.back();
        const core::Pointd2D p0(1300, 450);
        return new StepGameUnit(game, isoMap, unit, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // ADD ENEMY NEAR BASE
    AddStep([]
    {
        const core::Pointd2D p0(600, 150);
        return new StepGameEnemyIntro(p0);
    });
    AddStep([this, playerAI]
    {
        return new StepGameAddEnemy(GetGameMap(), playerAI, ObjectData::TYPE_UNIT_SOLDIER1,
                                    cellEnemy1, true);
    });
    // MOVE CAMERA
    AddStep([game]
    {
        const int movX = -250;
        const int movY = -250;
        return new StepGameMoveCamera(movX, movY);
    });
    // MOVE SPAWNER NEAR ENEMY
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([local, isoMap, game]
    {
        const auto all = local->GetUnitsByType(ObjectData::TYPE_UNIT_SPAWNER1);
        const auto unit = all.back();
        const Cell2D target(70, 63);
        const core::Pointd2D p0(1100, 300);
        return new StepGameMoveUnitSimple(game, unit, isoMap, target, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // SPAWN MINI UNITS
    AddStep([game, gs, panelActions]
        {
            const core::Pointd2D p0(200, 600);
            return new StepGameSpawnIcon(game, gs, panelActions, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([hud] { return new StepGameSetupMiniUnits(hud, { 2, 3 }, ObjectData::TYPE_MINI_UNIT1); });
    AddStep([local]
    {
        const auto all = local->GetUnitsByType(ObjectData::TYPE_UNIT_SPAWNER1);
        const auto unit = all.back();
        return new StepGameWaitObjectIdle(unit);
    });
    // SELECT MINI UNITS
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, game, isoMap]
    {
        const auto mu = GetObjectInCell(cellMiniUnits1);
        const core::Pointd2D p0(500, 250);
        return new StepGameSelectObject(game, isoMap, mu, "TUT_GAME_MU_1", p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // SET TARGET FOR MINI UNITS
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(200, 600);
        return new StepGameSetTargetIcon(game, gs, panelActions, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, isoMap]
    {
        const auto mu = GetObjectInCell(cellMiniUnits1);
        const auto group = static_cast<MiniUnitsGroup *>(mu->GetGroup());
        const Cell2D target(65,52);
        const core::Pointd2D p0(900, 150);
        return new StepGameSetTarget(isoMap, group, target, p0);
    });
    // END TURN
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([panelTurn] { return new StepGameEndTurnSimple(panelTurn); });
    // WAIT ENEMY KILL
    AddStep([this]
    {
        return new StepGameWaitEnemyKilled(GetObjectInCell(cellEnemy1), GetGameMap());
    });
    AddStep([this] { return new StepGameEnemyKilled(); });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA TO TOP
    AddStep([isoMap]
    {
        const Cell2D cell(48, 48);
        return new StepGameMoveCameraOverCell(cell, isoMap, speedCamera);
    });
    // BUILD WALL RIGHT 1
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 500);
        auto step = new StepGameBuildWallLine(gameMap, isoMap, wallRight1TL, wallRight1BR,
                                              "TUT_GAME_BUILD_WALL_5");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // CONNECT WALL RIGHT 1
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 500);
        auto step = new StepGameConnectWall(gameMap, isoMap, wallRight1TL, wallRight1BR,
                                            "TUT_GAME_CONNECT_WALL");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD BUNKER IN RIGHT WALL 1
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 600);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_BUNKER);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_BUNKER_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_BUNKER_1",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud,"TUT_GAME_BUILD_DTOWER_3", nullptr,
                                               "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catDefenses,
                                               TutorialConstants::structBunker);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const unsigned int numToBuild = 1;
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallRight1TL, wallRight1BR,
                                                    ObjectData::TYPE_BUNKER, numToBuild,
                                                    "TUT_GAME_BUILD_BUNKER_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD WALL RIGHT 2
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1200, 200);
        auto step = new StepGameBuildWallLine(gameMap, isoMap, wallRight2TL, wallRight2BR,
                                              "TUT_GAME_BUILD_WALL_5");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // CONNECT WALL LEFT 2
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 200);
        auto step = new StepGameConnectWall(gameMap, isoMap, wallRight2TL, wallRight2BR,
                                            "TUT_GAME_CONNECT_WALL");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD GATE IN RIGHT WALL 2
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 100);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_WALL_GATE);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_GATE_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_GATE_1",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud,"TUT_GAME_BUILD_DTOWER_3",
                                               "TUT_GAME_BUILD_GATE_2",
                                               "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catDefenses,
                                               TutorialConstants::structGate);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const unsigned int numToBuild = 1;
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallRight2TL, wallRight2BR,
                                                    ObjectData::TYPE_WALL_GATE, numToBuild,
                                                    "TUT_GAME_BUILD_GATE_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([] { return new StepDelay(0.5f); });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD TOWER IN RIGHT WALL 2
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 600);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_DEFENSIVE_TOWER);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_DTOWER_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_DTOWER_1",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud,"TUT_GAME_BUILD_DTOWER_3",
                                               "TUT_GAME_BUILD_GATE_2",
                                               "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catDefenses,
                                               TutorialConstants::structDefTower);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const unsigned int numToBuild = 1;
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallRight2TL, wallRight2BR,
                                                    ObjectData::TYPE_DEFENSIVE_TOWER, numToBuild,
                                                    "TUT_GAME_BUILD_DTOWER_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // INTRO MOVE WORKER TO MATERIAL GENERATOR 3
    AddStep([]
    {
        const core::Pointd2D p0(1300, 200);
        return new StepGameSingleInfo(p0, "TUT_GAME_CONT_EXPL");
    });
    // MOVE CAMERA OVER TARGET
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([isoMap]
    {
        const Cell2D cell(41, 69);
        return new StepGameMoveCameraOverCell(cell, isoMap, speedCamera);
    });
    // MOVE WORKER MATERIAL GENERATOR 3
    AddStep([gameMap, isoMap, local]
    {
        const core::Pointd2D p0(1300, 200);
        const Cell2D target(45, 73);
        return new StepGameMoveToCellFree(gameMap, isoMap, "TUT_GAME_MOVE_UNIT_6",
                                          target, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER MATERIAL GENERATOR 3
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen3);
        const core::Pointd2D p0(1300, 150);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_MAT_GEN", p0);
    });
    // CONNECT MATERIAL GENERATOR 2
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen3);
        const core::Pointd2D p0(1300, 150);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // INTRO MOVE WORKER TO ENERGY GENERATOR 3
    AddStep([]
    {
        const core::Pointd2D p0(1300, 200);
        return new StepGameSingleInfo(p0, "TUT_GAME_CONT_EXPL");
    });
    // MOVE CAMERA OVER TARGET
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([isoMap]
    {
        const Cell2D cell(65, 60);
        return new StepGameMoveCameraOverCell(cell, isoMap, speedCamera);
    });
    // MOVE WORKER ENERGY GENERATOR 3
    AddStep([gameMap, isoMap, local]
    {
        const core::Pointd2D p0(1300, 200);
        const Cell2D target(70, 57);
        return new StepGameMoveToCellFree(gameMap, isoMap, "TUT_GAME_MOVE_UNIT_6",
                                          target, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER MATERIAL GENERATOR 3
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen3);
        const core::Pointd2D p0(1300, 150);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_MAT_GEN", p0);
    });
    // CONNECT MATERIAL GENERATOR 2
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen3);
        const core::Pointd2D p0(1300, 150);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    /*
    // MOVE VIEW BACK TO BASE
    AddStep([panelTurn]
            {
                const core::Pointd2D p0(300, 550);
                return new StepGameBackToBase(panelTurn, "TUT_GAME_BACK_TO_BASE_1b", p0);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // COLLECT SECONDARY MISSION GOAL
    AddStep([localBase, game, isoMap]
            {
                const core::Pointd2D p0(500, 200);
                return new StepGameSelectObject(game, isoMap, localBase, "TUT_GAME_BASE_4", p0);
            });
    AddStep([game, gs, panelActions] { return new StepGameMissionGoalsIcon(game, gs, panelActions, false); });
    AddStep([hud]
        {
            const int goal = 1;
            return new StepGameSecondaryMissionGoal(hud, goal);
        });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    AddStep([] { return new StepDelay(0.5f); });
    // SELECT WORKER 1
    AddStep([hud]
        {
            const sgl::core::Pointd2D p0(200, 600);
            return new StepGameQuickUnitButton(hud, indWorker1, nullptr, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA
    AddStep([game]
        {
            const int movX = 400;
            const int movY = 0;
            return new StepGameMoveCamera(movX, movY);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD TRADING POST
    AddStep([game, gs, panelActions]
        {
            const core::Pointd2D p0(1100, 450);
            return new StepGameBuildStructIntro(game, gs, panelActions, "TUT_GAME_BUILD_TRAD_POST_1", p0);
        });
    AddStep([hud]
        {
            return new StepGameBuildStructure(hud, nullptr, "TUT_GAME_BUILD_TRAD_POST_2",
                                              TutorialConstants::catGeneric,
                                              TutorialConstants::structTradingPost);
        });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    AddStep([this, local, isoMap, game]
        {
            const auto unit = local->GetUnit(indWorker1);
            const core::Pointd2D p0(1100, 250);
            return new StepGameBuildTowerEnd(isoMap, unit, cellTradingPost, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
        // SELECT TRADING POST
    AddStep([this, game, isoMap]
        {
            const core::Pointd2D p0(710, 250);
            const GameObject * obj = GetObjectInCell(cellTradingPost);
            return new StepGameSelectObject(game, isoMap, obj, "TUT_GAME_BUILD_TRAD_POST_2", p0);
        });
    // OPEN AND USE DIALOG TRADING
    AddStep([game, gs, panelActions]
        {
            const core::Pointd2D p0(100, 650);
            return new StepGameTradeIcon(game, gs, panelActions, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([hud]
        {
            return new StepGameDialogTradingBuy(hud);
        });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    // END TURN
    AddStep([panelTurn] { return new StepGameEndTurnSimple(panelTurn); });
    AddStep([gs] { return new StepGameWaitTurn(gs); });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // SELECT RESEARCH CENTER AND UNLOCK UPGRADE
    AddStep([this]
            {
                const GameObject * obj = GetObjectInCell(cellResCenter);
                const float speed = 800.f;
                return new StepGameMoveCameraOverObject(obj, speed);
            });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([]
            {
                const core::Pointd2D p0(685, 200);
                return new StepGameSingleInfo(p0, "TUT_GAME_RES_CEN_2");
            });
    AddStep([this, game, isoMap]
            {
                const core::Pointd2D p0(710, 250);
                const GameObject * obj = GetObjectInCell(cellResCenter);
                return new StepGameSelectObject(game, isoMap, obj, "TUT_GAME_RES_CEN_1", p0);
            });
    AddStep([game, gs, panelActions]
            {
                const core::Pointd2D p0(1000, 650);
                return new StepGameTechTreeIcon(game, gs, panelActions, p0);
            });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([hud] { return new StepGameTechTreeDialog(hud, TECH_UP_STORAGE_STRUCTS, false); });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    AddStep([] { return new StepDelay(0.5f); });
    // SELECT WORKER 2
    AddStep([hud]
        {
            const sgl::core::Pointd2D p0(200, 600);
            return new StepGameQuickUnitButton(hud, indWorker2, nullptr, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER ENERGY GENERATOR WITH WORKER 2
    AddStep([this, local]
        {
            const GameObject * gen = GetObjectInCell(cellEneGen4);
            const core::Pointd2D p0(800, 150);
            return new StepGameConquerEnergyGenIntro(gen, p0);
        });
    AddStep([this, local, isoMap, game]
        {
            const auto unit = local->GetUnit(indWorker2);
            const GameObject * gen = GetObjectInCell(cellEneGen4);
            const core::Pointd2D p0(850, 200);
            return new StepGameConquerStructSimple(game, unit, gen, isoMap, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER CELLS WITH WORKER 2
    AddStep([]
            {
                const core::Pointd2D p0(1100, 350);
                return new StepGameConnectStructIntro(p0);
            });
    AddStep([game, gs, panelActions] { return new StepGameUnitConquerCellsIcon(game, gs, panelActions); });
    AddStep([this, isoMap, game]
            {
                const core::Pointd2D p0(1100, 400);
                const Cell2D & cellStart = GetOverlayCellConquest()->GetCellStart();
                const Cell2D target(41, 80);
                return new StepGameConquerCellsSimple(game, isoMap, cellStart, target, p0);
            });
    AddStep([this, local, isoMap, game]
            {
                const auto unit = local->GetUnit(indWorker2);
                const core::Pointd2D p0(400, 300);
                const Cell2D cellEnd(46, 79);
                return new StepGameConquerCellsEnd(game, isoMap, unit, cellEnd, p0);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // SELECT WORKER 1
    AddStep([hud]
        {
            const sgl::core::Pointd2D p0(200, 600);
            return new StepGameQuickUnitButton(hud, indWorker1, nullptr, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // TELL ABOUT MOVE
    AddStep([]
        {
            const core::Pointd2D p0(500, 250);
            return new StepGameSingleInfo(p0, "TUT_GAME_CONT_EXPL");
        });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA
    AddStep([game]
        {
            const int movX = -800;
                const int movY = 0;
                return new StepGameMoveCamera(movX, movY);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE TO MATERIAL GENERATOR
    AddStep([local, isoMap, game]
        {
            const auto unit = local->GetUnit(indWorker1);
            const Cell2D target(79, 37);
            const core::Pointd2D p0(700, 150);
            return new StepGameMoveUnitSimple(game, unit, isoMap, target, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER MATERIAL GENERATOR WITH WORKER 1
    AddStep([this, local]
        {
            const GameObject * gen = GetObjectInCell(cellMatGen4);
            const core::Pointd2D p0(1250, 350);
            return new StepGameConquerMaterialGenIntro(gen, p0);
        });
    AddStep([this, local, isoMap, game]
        {
            const auto unit = local->GetUnit(indWorker1);
            const GameObject * gen = GetObjectInCell(cellMatGen4);
            const core::Pointd2D p0(400, 200);
            return new StepGameConquerStructSimple(game, unit, gen, isoMap, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // ENABLE AI
    AddStep([playerAI] { return new StepAISetActive(playerAI->GetAI(), true); });
    // END TURN
    AddStep([panelTurn] { return new StepGameEndTurnSimple(panelTurn); });
    AddStep([gs] { return new StepGameWaitTurn(gs); });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER CELLS WITH WORKER 1
    AddStep([]
            {
                const core::Pointd2D p0(1100, 350);
                return new StepGameConnectStructIntro(p0);
            });
    AddStep([game, gs, panelActions] { return new StepGameUnitConquerCellsIcon(game, gs, panelActions); });
    AddStep([this, isoMap, game]
            {
                const core::Pointd2D p0(1100, 400);
                const Cell2D & cellStart = GetOverlayCellConquest()->GetCellStart();
                const Cell2D target(80, 37);
                return new StepGameConquerCellsSimple(game, isoMap, cellStart, target, p0);
            });
    AddStep([this, local, isoMap, game]
            {
                const auto unit = local->GetUnit(indWorker1);
                const core::Pointd2D p0(400, 300);
                const Cell2D cellEnd(79, 41);
                return new StepGameConquerCellsEnd(game, isoMap, unit, cellEnd, p0);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // SELECT WORKER 2
    AddStep([hud]
        {
            const sgl::core::Pointd2D p0(200, 600);
            return new StepGameQuickUnitButton(hud, indWorker2, nullptr, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD MATERIAL WAREHOUSE WITH WORKER 2
    AddStep([game, gs,panelActions]
        {
            const core::Pointd2D p0(1100, 250);
            return new StepGameBuildStructIntro(game, gs, panelActions, "TUT_GAME_BUILD_STORAGE", p0);
        });
    AddStep([hud]
        {
            return new StepGameBuildStructure(hud, "TUT_GAME_CAT_RES_GEN", "TUT_GAME_BUILD_STORAGE_1",
                                              TutorialConstants::catResources,
                                              TutorialConstants::structMatWarehouse);
        });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    AddStep([this, local, isoMap, game]
        {
            const auto unit = local->GetUnit(indWorker2);
            const core::Pointd2D p0(1000, 200);
            const Cell2D target(48, 73);
            return new StepGameBuildTowerEnd(isoMap, unit, target, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // UPGRADE WORKER 2
    AddStep([game, gs, panelActions]
            {
                core::Pointd2D p0(1000, 300);
                return new StepGameUpgradeIntro(game, gs, panelActions, "TUT_GAME_UPGRADE_1b", p0);
            });
    AddStep([hud] { return new StepGameUpgradeUnitFree(hud); });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    AddStep([] { return new StepDelay(0.5f); });
    // END TURN
    AddStep([panelTurn] { return new StepGameEndTurnSimple(panelTurn); });
    AddStep([gs] { return new StepGameWaitTurn(gs); });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // SELECT WORKER 1
    AddStep([hud]
        {
            const sgl::core::Pointd2D p0(200, 600);
            return new StepGameQuickUnitButton(hud, indWorker1, nullptr, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER CELLS WITH WORKER 1
    AddStep([game, gs, panelActions] { return new StepGameUnitConquerCellsIcon(game, gs, panelActions); });
    AddStep([this, isoMap, game]
            {
                const core::Pointd2D p0(1100, 400);
                const Cell2D & cellStart = GetOverlayCellConquest()->GetCellStart();
                const Cell2D target(79, 42);
                return new StepGameConquerCellsSimple(game, isoMap, cellStart, target, p0);
            });
    AddStep([this, local, isoMap, game]
            {
                const auto unit = local->GetUnit(indWorker1);
                const core::Pointd2D p0(400, 300);
                const Cell2D cellEnd(79, 46);
                return new StepGameConquerCellsEnd(game, isoMap, unit, cellEnd, p0);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD ENERGY WAREHOUSE
    AddStep([game, gs,panelActions]
        {
            const core::Pointd2D p0(1100, 250);
            return new StepGameBuildStructIntro(game, gs, panelActions, "TUT_GAME_BUILD_STORAGE", p0);
        });
    AddStep([hud]
        {
            return new StepGameBuildStructure(hud, "TUT_GAME_CAT_RES_GEN", "TUT_GAME_BUILD_STORAGE_2",
                                              TutorialConstants::catResources,
                                              TutorialConstants::structEneWarehouse);
        });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    AddStep([this, local, isoMap, game]
        {
            const auto unit = local->GetUnit(indWorker1);
            const core::Pointd2D p0(1000, 200);
            const Cell2D target(74, 50);
            return new StepGameBuildTowerEnd(isoMap, unit, target, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // END TURN
    AddStep([panelTurn] { return new StepGameEndTurnSimple(panelTurn); });
    AddStep([gs] { return new StepGameWaitTurn(gs); });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // ANNOUNCE NEW MINI UNITS
    AddStep([]
        {
            const core::Pointd2D p0(685, 250);
            return new StepGameSingleInfo(p0, "TUT_GAME_BUILD_MU");
        });
    // SELECT SPAWNER 1
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([hud]
        {
            const sgl::core::Pointd2D p0(100, 600);
            return new StepGameQuickUnitButton(hud, indSpawner1, nullptr, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // SPAWN MINI UNITS
    AddStep([game, gs, panelActions]
        {
            const core::Pointd2D p0(200, 600);
            return new StepGameSpawnIcon(game, gs, panelActions, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([hud] { return new StepGameSetupMiniUnits(hud, { 5, 3 }, ObjectData::TYPE_MINI_UNIT2); });
    AddStep([local]
        {
            const auto unit = local->GetUnit(indSpawner1);
            return new StepGameWaitObjectIdle(unit);
        });
    // FINAL MESSAGE
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([game]
        {
            const core::Pointd2D p0(635, 200);
            return new StepGameEnding(game, p0);
        });
    // FINAL SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    */
}

} // namespace game
