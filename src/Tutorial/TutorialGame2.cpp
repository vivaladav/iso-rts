#include "Tutorial/TutorialGame2.h"

#include "CameraMapController.h"
#include "Game.h"
#include "GameMap.h"
#include "Player.h"
#include "GameObjects/Barracks.h"
#include "GameObjects/Base.h"
#include "GameObjects/ObjectsDataRegistry.h"
#include "GameObjects/Unit.h"
#include "Indicators/OverlayCellConquest.h"
#include "Indicators/OverlayWall.h"
#include "Screens/ScreenGame.h"
#include "Tutorial/StepAISetActive.h"
#include "Tutorial/StepDelay.h"
#include "Tutorial/StepGameAddEnemy.h"
#include "Tutorial/StepGameIntro2.h"
#include "Tutorial/StepGameBackToBase.h"
#include "Tutorial/StepGameBuildStructArea.h"
#include "Tutorial/StepGameBuildStructAreaFree.h"
#include "Tutorial/StepGameBuildStructIntro.h"
#include "Tutorial/StepGameBuildStructure.h"
#include "Tutorial/StepGameBuildTowerEnd.h"
#include "Tutorial/StepGameBuildUnitEnd.h"
#include "Tutorial/StepGameBuildUnitStart.h"
#include "Tutorial/StepGameBuildWallLine.h"
#include "Tutorial/StepGameClearSelection.h"
#include "Tutorial/StepGameClickActionButton.h"
#include "Tutorial/StepGameCollectObjects.h"
#include "Tutorial/StepGameConnectStructFree.h"
#include "Tutorial/StepGameConnectWall.h"
#include "Tutorial/StepGameConnectStructIntro.h"
#include "Tutorial/StepGameConquerCellsEnd.h"
#include "Tutorial/StepGameConquerCellsSimple.h"
#include "Tutorial/StepGameConquerGeneratorIntro.h"
#include "Tutorial/StepGameConquerEnergyGenIntro.h"
#include "Tutorial/StepGameConquerMaterialGenIntro.h"
#include "Tutorial/StepGameConquerStructChoice.h"
#include "Tutorial/StepGameConquerStructFree.h"
#include "Tutorial/StepGameConquerStructSimple.h"
#include "Tutorial/StepGameDialogTrading.h"
#include "Tutorial/StepGameDisableCamera.h"
#include "Tutorial/StepGameEnableCamera.h"
#include "Tutorial/StepGameEndTurnSimple.h"
#include "Tutorial/StepGameEnemyIntro.h"
#include "Tutorial/StepGameInfoArea.h"
#include "Tutorial/StepGameMakeEnemyAttack.h"
#include "Tutorial/StepGameMissionGoalsIcon.h"
#include "Tutorial/StepGameMoveCameraOverCell.h"
#include "Tutorial/StepGameMoveCameraOverObject.h"
#include "Tutorial/StepGameMoveUnitFree.h"
#include "Tutorial/StepGameMoveUnitSimple.h"
#include "Tutorial/StepGameMoveUnitToArea.h"
#include "Tutorial/StepGamePanelHit.h"
#include "Tutorial/StepGamePrimaryMissionGoal.h"
#include "Tutorial/StepGameQuickUnitButton.h"
#include "Tutorial/StepGameResourcesBar.h"
#include "Tutorial/StepGameSecondaryMissionGoal.h"
#include "Tutorial/StepGameSelectObject.h"
#include "Tutorial/StepGameSetCollectableGeneratorTurns.h"
#include "Tutorial/StepGameSetCollectableUnits.h"
#include "Tutorial/StepGameSetObjectAttackMode.h"
#include "Tutorial/StepGameSetObjectEnergy.h"
#include "Tutorial/StepGameSetObjectFatalHit.h"
#include "Tutorial/StepGameSetObjectHealth.h"
#include "Tutorial/StepGameSetObjectIndestructible.h"
#include "Tutorial/StepGameSetObjectPerfectShot.h"
#include "Tutorial/StepGameSetSelectionDefaultAction.h"
#include "Tutorial/StepGameSetSelectionActiveAction.h"
#include "Tutorial/StepGameSetupResearch.h"
#include "Tutorial/StepGameSetupResearchIcon.h"
#include "Tutorial/StepGameSingleInfo.h"
#include "Tutorial/StepGameTechTreeDialog.h"
#include "Tutorial/StepGameTechTreeIcon.h"
#include "Tutorial/StepGameTradeIcon.h"
#include "Tutorial/StepGameUnit.h"
#include "Tutorial/StepGameUnitAttackBurst.h"
#include "Tutorial/StepGameUnitAttackIcon.h"
#include "Tutorial/StepGameUnitAttackContinue.h"
#include "Tutorial/StepGameUnitAttackFree.h"
#include "Tutorial/StepGameUnitAttackSimple.h"
#include "Tutorial/StepGameUnitConquerCellsIcon.h"
#include "Tutorial/StepGameUpgradeIntro.h"
#include "Tutorial/StepGameUpgradeUnit.h"
#include "Tutorial/StepGameUpgradeUnitFree.h"
#include "Tutorial/StepGameWaitEnemiesKilled.h"
#include "Tutorial/StepGameWaitEnemyKilled.h"
#include "Tutorial/StepGameWaitObjectDestroyed.h"
#include "Tutorial/StepGameWaitTurn.h"
#include "Tutorial/StepGameWaitUnitCanBuild.h"
#include "Tutorial/StepGameWallBuildEnd.h"
#include "Tutorial/StepGameWallBuildIcon.h"
#include "Tutorial/StepGameWallBuildIntro.h"
#include "Tutorial/StepGameWallBuildStart.h"
#include "Tutorial/StepSaveGame.h"
#include "Tutorial/TutorialConstants.h"
#include "Widgets/GameHUD.h"
#include "Widgets/PanelObjectActions.h"

#include <vector>

namespace
{
using namespace game;

constexpr unsigned int indWorker1 = 0;
constexpr unsigned int indSoldier1 = 1;
constexpr unsigned int indWorker2 = 2;
constexpr unsigned int indSoldier2 = 3;

constexpr int turnsCollGenMin = 2;
constexpr int turnsCollGenMax = 2;
constexpr int turnsCollGenMin2 = 5;
constexpr int turnsCollGenMax2 = 15;
constexpr int collectablesMin = 2;
constexpr int collectablesMax = 4;

constexpr float healthEnemyDef = 65.f;
constexpr float energyEnemyDef = 60.f;

constexpr float delayAnimLong = 2.f;

constexpr float speedCamera = 700.f;

const Cell2D cellEneGen1(6, 15);
const Cell2D cellEneGen2(23, 17);
const Cell2D cellEneGen3(5, 26);
const Cell2D cellEneGen4(13, 43);
const Cell2D cellMatGen1(15, 7);
const Cell2D cellMatGen2(24, 3);
const Cell2D cellMatGen3(15, 26);
const Cell2D cellMatGen4(40, 12);
const Cell2D cellBarracks(8, 9);
const Cell2D cellTarget1(15, 13);
const Cell2D cellResCenter(11, 6);
const Cell2D cellTradingPost(17, 11);
const Cell2D cellEnemy1(4, 34);
const Cell2D cellEnemy2(30, 19);
const Cell2D cellEnemy3(29, 24);
const Cell2D cellEnemy4(21, 32);
const Cell2D cellEnemy5(25, 30);
const Cell2D cellTower1(6, 33);
const Cell2D cellTower2(17, 31);
const Cell2D cellTower3(28, 17);
const Cell2D cellTower4(21, 29);
const Cell2D cellTower5(27, 21);
const Cell2D cellTower6(11, 32);
const Cell2D cellTower7(24, 23);
const Cell2D cellTower8(23, 27);
const Cell2D cellTower9(28, 9);

const Cell2D areaBlobsTL(22, 12);
const Cell2D areaBlobsBR(25, 13);
const Cell2D areaBlobs2TL(2, 46);
const Cell2D areaBlobs2BR(4, 48);
const Cell2D areaDiamondsTL(23, 6);
const Cell2D areaDiamondsBR(24, 9);
const Cell2D areaDiamonds2TL(7,41);
const Cell2D areaDiamonds2BR(9, 42);

const Cell2D areaTreesTL(13, 13);
const Cell2D areaTreesBR(16, 16);

const Cell2D targetSoldier1(15, 11);

const Cell2D wallRightTL(12, 29);
const Cell2D wallRightBR(22, 29);
const Cell2D wallCenterTL(24, 23);
const Cell2D wallCenterBR(24, 28);
const Cell2D wallLeftTL(27, 12);
const Cell2D wallLeftBR(27, 21);

}

namespace game
{

TutorialGame2::TutorialGame2(Screen * screen)
    : TutorialGame(screen, TUTORIAL_MISSION_2)
{
    using namespace sgl;

    auto gs = GetScreen();
    auto game = gs->GetGame();
    auto hud = gs->GetHUD();
    auto gameMap = GetGameMap();
    auto isoMap = GetIsoMap();

    const Player * local = game->GetActivePlayerByIndex(0);
    Player * playerAI = game->GetActivePlayerByIndex(1);

    auto localBase = local->GetBase();
    auto panelActions = hud->GetPanelObjectActions();
    auto panelTurn = hud->GetPanelTurnControl();

    // keep camera enabled in this tutorial
    GetCameraMapController()->SetEnabled(true);

    // ===== SETUP =====
    // set all generators of diamonds and blobs to create them within 2 turns
    AddStep([gameMap]
        {
            return new StepGameSetCollectableGeneratorTurns(areaDiamondsTL, areaDiamondsBR, gameMap,
                                                            ObjectData::TYPE_DIAMONDS,
                                                            turnsCollGenMin, turnsCollGenMax);
        });
    AddStep([gameMap]
        {
            return new StepGameSetCollectableGeneratorTurns(areaDiamonds2TL, areaDiamonds2BR, gameMap,
                                                            ObjectData::TYPE_DIAMONDS,
                                                            turnsCollGenMin, turnsCollGenMax);
        });
    AddStep([gameMap]
        {
            return new StepGameSetCollectableGeneratorTurns(areaBlobsTL, areaBlobsBR, gameMap,
                                                            ObjectData::TYPE_BLOBS,
                                                            turnsCollGenMin, turnsCollGenMax);
        });
    AddStep([gameMap]
        {
            return new StepGameSetCollectableGeneratorTurns(areaBlobs2TL, areaBlobs2BR, gameMap,
                                                            ObjectData::TYPE_BLOBS,
                                                            turnsCollGenMin, turnsCollGenMax);
        });
    // MAKE TREES INDESTRUCTIBLE
    AddStep([gameMap]
    {
        std::vector<GameObject *> trees;
        gameMap->GetObjectsInAreaByType(trees, areaTreesTL, areaTreesBR, ObjectData::TYPE_TREES3);
        return new StepGameSetObjectIndestructible(std::move(trees), true);
    });
    // pause before start
    AddStep([] { return new StepDelay(0.5f); });

    // ===== PART 1 =====
    // INTRO
    AddStep([] { return new StepGameIntro2; });
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
    // CONQUER ENERGY GENERATOR 1
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen1);
        const core::Pointd2D p0(100, 450);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_ENE_GEN", p0);
    });
    // CONNECT ENERGY GENERATOR 1
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen1);
        const core::Pointd2D p0(1200, 400);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER MATERIAL GENERATOR 1
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen1);
        const core::Pointd2D p0(1200, 400);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_MAT_GEN", p0);
    });
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
    // BUILD BARRACKS
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 100);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_BARRACKS);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_BARRACKS_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_BARRACKS_1",
                                                  PanelObjectActions::BTN_BUILD_STRUCT, delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildStructure(hud, nullptr, nullptr, "TUT_GAME_BASE_BUILD_UNIT_3",
                                               TutorialConstants::catGeneric,
                                               TutorialConstants::structBarracks);
        step->SetDelayAnimation(2.f);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const Cell2D areaTL(7, 8);
        const Cell2D areaBR(11, 14);

        auto step = new StepGameBuildStructArea(game, isoMap, areaTL, areaBR,
                                                "TUT_GAME_BUILD_BARRACKS_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // CONNECT BARRACKS IF NEEDED
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const auto all = local->GetStructuresByType(ObjectData::TYPE_BARRACKS);
        const GameObject * barracks = all.back();
        const core::Pointd2D p0(1200, 400);
        return new StepGameConnectStructFree(game, barracks, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD SOLDIER
    AddStep([local, game, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const auto all = local->GetStructuresByType(ObjectData::TYPE_BARRACKS);
        const GameObject * barracks = all.back();

        return new StepGameSelectObject(game, isoMap, barracks, "TUT_GAME_BUILD_LSOLDIER", p0,
                                        delayAnimLong);
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_LSOLDIER",
                                                  PanelObjectActions::BTN_BUILD_UNIT_BARRACKS,
                                                  delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([hud]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameBuildUnitEnd(hud, "TUT_GAME_BUILD_LSOLDIER", delayAnimLong);
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([local]
    {
        const auto all = local->GetStructuresByType(ObjectData::TYPE_BARRACKS);
        const auto barracks = static_cast<Barracks *>(all.back());
        return new StepDelay(barracks->GetTimeBuildUnit());
    });
    // MOVE SOLDIER NEAR TREES
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([game, isoMap, local]
    {
        const core::Pointd2D p0(1300, 500);
        const auto all = local->GetUnitsByType(ObjectData::TYPE_UNIT_SOLDIER1);
        const auto unit = all.back();
        return new StepGameMoveUnitFree(game, unit, isoMap, "TUT_GAME_MOVE_SOLDIER",
                                        targetSoldier1, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // MAKE TREES DESTRUCTIBLE AGAIN
    AddStep([gameMap]
    {
        std::vector<GameObject *> trees;
        gameMap->GetObjectsInAreaByType(trees, areaTreesTL, areaTreesBR, ObjectData::TYPE_TREES3);
        return new StepGameSetObjectIndestructible(std::move(trees), false);
    });
    // EXPLAIN ATTACK
    AddStep([]
        {
            const core::Pointd2D p0(300, 500);
            return new StepGameSingleInfo(p0, "TUT_GAME_ATTACK_1");
        });
    AddStep([game, gs, panelActions]
        {
            const core::Pointd2D p0(450, 650);
            return new StepGameUnitAttackIcon(game, gs, panelActions, p0);
        });
    AddStep([hud]
        {
            const core::Pointd2D p0(150, 600);

            auto panelShot = hud->GetPanelShotType();
            return new StepGameUnitAttackBurst(panelShot, "TUT_GAME_ATTACK_3", p0);
        });
    AddStep([isoMap]
    {
        const core::Pointd2D p0(200, 600);
        return new StepGamePanelHit(isoMap, areaTreesBR, areaTreesTL, p0);
    });
    // LOWER TREE'S HEALTH
    AddStep([this]
    {
        const float health = 20.f;
        GameObject * enemy = GetObjectInCell(cellTarget1);
        return new StepGameSetObjectHealth(enemy, health);
    });
    // ATTACK TREE
    AddStep([this, isoMap, game, gameMap]
    {
        const auto unit = static_cast<Unit *>(gameMap->GetObject(targetSoldier1.row,
                                                                 targetSoldier1.col));
        const GameObject * enemy = GetObjectInCell(cellTarget1);
        const core::Pointd2D p0(250, 500);

        auto step = new StepGameUnitAttackFree(game, unit, enemy, isoMap,
                                               gameMap, "TUT_GAME_ATTACK_6");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    AddStep([this, isoMap, game, gameMap]
    {
        const GameObject * enemy = GetObjectInCell(cellTarget1);
        const core::Pointd2D p0(250, 500);

        auto step = new StepGameWaitObjectDestroyed(game, enemy, isoMap, gameMap,
                                                    "TUT_GAME_DESTROY_TREE");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // PRESENT DIAMONDS
    AddStep([this, local, isoMap]
    {
        const core::Pointd2D p0(700, 600);
        return new StepGameInfoArea(isoMap, areaDiamondsTL, areaDiamondsBR,
                                    "TUT_GAME_MOVE_UNIT_4a", "TUT_GAME_MOVE_UNIT_4b", p0);
    });
    // COLLECT DIAMONDS
    AddStep([isoMap, gameMap, local, game]
    {
        const core::Pointd2D p0(1300, 400);
        return new StepGameCollectObjects(ObjectData::TYPE_DIAMONDS, gameMap, isoMap,
                                          areaDiamondsTL, areaDiamondsBR,
                                          "TUT_GAME_COLLECT_DIAMONDS", p0);
    });
    // MOVE CAMERA OVER BLOBS
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([isoMap]
    {
        const Cell2D cell(20, 11);
        return new StepGameMoveCameraOverCell(cell, isoMap, speedCamera);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // PRESENT BLOBS
    AddStep([this, local, isoMap]
    {
        const core::Pointd2D p0(1200, 600);
        return new StepGameInfoArea(isoMap, areaBlobsTL, areaBlobsBR,
                                    "TUT_GAME_MOVE_UNIT_5a", "TUT_GAME_MOVE_UNIT_5b", p0);
    });
    // COLLECT BLOBS
    AddStep([isoMap, gameMap, local, game]
    {
        const core::Pointd2D p0(1300, 600);
        return new StepGameCollectObjects(ObjectData::TYPE_BLOBS, gameMap, isoMap,
                                          areaBlobsTL, areaBlobsBR,
                                          "TUT_GAME_COLLECT_BLOBS", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA OVER MATERIAL GENERATOR
    AddStep([this]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen2);
        return new StepGameMoveCameraOverObject(gen, speedCamera);
    });
    // CONQUER MATERIAL GENERATOR 2
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen2);
        const core::Pointd2D p0(1200, 400);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_MAT_GEN", p0);
    });
    // CONNECT MATERIAL GENERATOR 2
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen2);
        const core::Pointd2D p0(1300, 400);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA OVER ENERGY GENERATOR
    AddStep([this]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen2);
        return new StepGameMoveCameraOverObject(gen, speedCamera);
    });
    // CONQUER ENERGY GENERATOR 2
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen2);
        const core::Pointd2D p0(100, 450);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_ENE_GEN", p0);
    });
    // CONNECT ENERGY GENERATOR 2
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen2);
        const core::Pointd2D p0(1200, 400);
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
    AddStep([game, isoMap]
    {
        const core::Pointd2D p0(1300, 100);
        const Cell2D areaTL(7, 8);
        const Cell2D areaBR(12, 16);

        auto step = new StepGameBuildStructArea(game, isoMap, areaTL, areaBR,
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
        return new StepGameSetupResearch(hud, { 100, 90, 80 });
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // EXPLAIN RESOURCES BAR
    AddStep([hud] { return new StepGameResourcesBar(hud); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE SOLDIER UNIT TO RIGHT
    AddStep([]
    {
        const core::Pointd2D p0(1300, 200);
        return new StepGameSingleInfo(p0, "TUT_GAME_MOVE_UNIT_3");
    });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([] { return new StepGameMoveCamera(0.f, 250.f); });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([game, isoMap, local]
    {
        const core::Pointd2D p0(1300, 500);
        const Cell2D target(13,21);
        const auto all = local->GetUnitsByType(ObjectData::TYPE_UNIT_SOLDIER1);
        const auto unit = all.back();
        return new StepGameMoveUnitFree(game, unit, isoMap, "TUT_GAME_MOVE_SOLDIER",
                                        target, p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
     // CONQUER MATERIAL GENERATOR 3
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen3);
        const core::Pointd2D p0(1200, 400);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_MAT_GEN", p0);
    });
    // CONNECT MATERIAL GENERATOR 3
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellMatGen3);
        const core::Pointd2D p0(1300, 400);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // CONQUER ENERGY GENERATOR 3
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen3);
        const core::Pointd2D p0(100, 450);
        return new StepGameConquerStructFree(game, gen, isoMap, "TUT_GAME_CONQUER_ENE_GEN", p0);
    });
    // CONNECT ENERGY GENERATOR 3
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([this, local, isoMap, game]
    {
        const GameObject * gen = GetObjectInCell(cellEneGen3);
        const core::Pointd2D p0(1200, 300);
        return new StepGameConnectStructFree(game, gen, isoMap, "TUT_GAME_CONNECT_STRUCT", p0);
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA
    AddStep([isoMap]
    {
        const Cell2D target(15, 29);
        return new StepGameMoveCameraOverCell(target, isoMap, speedCamera);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD WALL RIGHT
    AddStep([]
    {
        const core::Pointd2D p0(1300, 450);
        return new StepGameWallBuildIntro(p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 450);
        auto step = new StepGameBuildWallLine(gameMap, isoMap, wallRightTL, wallRightBR,
                                              "TUT_GAME_BUILD_WALL_5");
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
    AddStep([hud] { return new StepGameTechTreeDialog(hud, TECH_UP_BASE_IMPROVE_1, true); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA
    AddStep([isoMap]
    {
        const Cell2D target((wallRightTL.row + wallRightBR.row) / 2, wallRightTL.col);
        return new StepGameMoveCameraOverCell(target, isoMap, speedCamera);
    });
    // CONNECT WALL RIGHT
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 350);
        auto step = new StepGameConnectWall(gameMap, isoMap, wallRightTL, wallRightBR,
                                            "TUT_GAME_CONNECT_WALL");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // BUILD GATE IN RIGHT WALL
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
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallRightTL, wallRightBR,
                                                    ObjectData::TYPE_WALL_GATE, numToBuild,
                                                    "TUT_GAME_BUILD_GATE_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD TOWERS IN RIGHT WALL
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 600);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_DEFENSIVE_TOWER);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_DTOWERS");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_DTOWERS",
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
        const unsigned int numToBuild = 2;
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallRightTL, wallRightBR,
                                                    ObjectData::TYPE_DEFENSIVE_TOWER, numToBuild,
                                                    "TUT_GAME_BUILD_DTOWERS");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA
    AddStep([isoMap]
    {
        return new StepGameMoveCameraOverCell(wallCenterBR, isoMap, speedCamera);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD WALL CENTRAL
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 450);
        auto step = new StepGameBuildWallLine(gameMap, isoMap, wallCenterTL, wallCenterBR,
                                              "TUT_GAME_BUILD_WALL_5");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // CONNECT WALL CENTER
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 150);
        auto step = new StepGameConnectWall(gameMap, isoMap, wallCenterTL, wallCenterBR,
                                            "TUT_GAME_CONNECT_WALL");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // BUILD TOWERS IN CENTRAL WALL
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 600);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_DEFENSIVE_TOWER);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_DTOWERS");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_DTOWERS",
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
        const unsigned int numToBuild = 2;
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallCenterTL, wallCenterBR,
                                                    ObjectData::TYPE_DEFENSIVE_TOWER, numToBuild,
                                                    "TUT_GAME_BUILD_DTOWERS");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA
    AddStep([isoMap]
    {
        return new StepGameMoveCameraOverCell(wallLeftBR, isoMap, speedCamera);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD WALL LEFT
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(100, 500);
        auto step = new StepGameBuildWallLine(gameMap, isoMap, wallLeftTL, wallLeftBR,
                                              "TUT_GAME_BUILD_WALL_5");
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
    AddStep([hud] { return new StepGameTechTreeDialog(hud, TECH_UP_TRADING_POST, true); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE CAMERA
    AddStep([isoMap]
    {
        return new StepGameMoveCameraOverCell(wallLeftBR, isoMap, speedCamera);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // CONNECT WALL LEFT
    AddStep([gameMap, isoMap]
    {
        const core::Pointd2D p0(1300, 350);
        auto step = new StepGameConnectWall(gameMap, isoMap, wallLeftTL, wallLeftBR,
                                            "TUT_GAME_CONNECT_WALL");
        step->SetInfoPosition(p0.x, p0.y);
        return step;
    });
    // BUILD GATE IN LEFT WALL
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
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallLeftTL, wallLeftBR,
                                                    ObjectData::TYPE_WALL_GATE, numToBuild,
                                                    "TUT_GAME_BUILD_GATE_1");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD TOWERS IN LEFT WALL
    AddStep([local, game]
    {
        const core::Pointd2D p0(1300, 600);

        const auto reg = game->GetObjectsRegistry();
        const ObjectData & data = reg->GetObjectData(ObjectData::TYPE_DEFENSIVE_TOWER);

        auto step = new StepGameWaitUnitCanBuild(local, data, "TUT_GAME_BUILD_DTOWERS");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    AddStep([game, gs, panelActions]
    {
        const core::Pointd2D p0(1300, 100);
        auto step = new StepGameClickActionButton(game, gs, panelActions, "TUT_GAME_BUILD_DTOWERS",
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
        const unsigned int numToBuild = 2;
        auto step = new StepGameBuildStructAreaFree(gameMap, isoMap, wallLeftTL, wallLeftBR,
                                                    ObjectData::TYPE_DEFENSIVE_TOWER, numToBuild,
                                                    "TUT_GAME_BUILD_DTOWERS");
        step->SetInfoPosition(p0.x, p0.y);

        return step;
    });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });

    /*
    // MOVE VIEW BACK TO BASE
    AddStep([panelTurn, game]
    {
        const core::Pointd2D p0(50, 700);
        return new StepGameBackToBase(panelTurn, "TUT_GAME_BACK_TO_BASE_1", p0);
    });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD WORKER 2
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
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    */


    /*
    // MOVE WORKER AWAY FROM WALL
    AddStep([game]
        {
            const int movX = -300;
            const int movY = 0;
            return new StepGameMoveCamera(movX, movY);
        });
    AddStep([this, local, isoMap, game]
        {
            const auto unit = local->GetUnit(indWorker1);
            const Cell2D target(20, 28);
            const core::Pointd2D p0(300, 400);
            return new StepGameMoveUnitSimple(game, unit, isoMap, target, p0);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // ADD ENEMY NEAR GATE 1
    AddStep([]
        {
            const core::Pointd2D p0(1100, 400);
            return new StepGameEnemyIntro(p0);
        });
    AddStep([this, playerAI]
        {
            return new StepGameAddEnemy(GetGameMap(), playerAI, ObjectData::TYPE_UNIT_SCOUT1,
                                        cellEnemy1, true);
        });
    // CENTER VIEW ON SOLDIER
    AddStep([hud]
        {
            const sgl::core::Pointd2D p0(200, 700);
            return new StepGameQuickUnitButton(hud, indSoldier1, nullptr, p0);
        });
    // DISABLE PERFECT SHOT AND FATAL HIT
    AddStep([local]
        {
            auto unit = local->GetUnit(indSoldier1);
            return new StepGameSetObjectPerfectShot(unit, false);
        });
    AddStep([local]
        {
            auto unit = local->GetUnit(indSoldier1);
            return new StepGameSetObjectFatalHit(unit, false);
        });
    AddStep([] { return new StepDelay(0.5f); });
    // ATTACK ENEMY WITH SOLDIER
    AddStep([]
            {
                const core::Pointd2D p0(700, 200);
                return new StepGameSingleInfo(p0, "TUT_GAME_ATTACK_1b");
            });
    AddStep([game, gs, panelActions]
            {
                const core::Pointd2D p0(700, 250);
                return new StepGameUnitAttackIcon(game, gs, panelActions, p0);
            });
    AddStep([this, local, isoMap, game]
            {
                const auto unit = local->GetUnit(indSoldier1);
                const core::Pointd2D p0(700, 250);
                return new StepGameUnitAttackContinue(game, unit, isoMap, cellEnemy1, p0);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // ENABLE TOWER'S PERFECT SHOT
    AddStep([this]
            {
                auto tower = GetObjectInCell(cellTower1);
                return new StepGameSetObjectPerfectShot(tower, true);
            });
    // LOWER ENEMY HEALTH, SET ATTACK TO BURST AND DISABLE FATAL HIT
    AddStep([this]
            {
                const float health = 20.f;
                GameObject * enemy = GetObjectInCell(cellEnemy1);
                return new StepGameSetObjectHealth(enemy, health);
            });
    AddStep([this]
            {
                auto enemy = GetObjectInCell(cellEnemy1);
                return new StepGameSetObjectAttackMode(enemy, ATT_BURST_SHOT);
            });
    AddStep([this]
            {
                auto enemy = GetObjectInCell(cellEnemy1);
                return new StepGameSetObjectFatalHit(enemy, false);
            });
    // END TURN - NOTE do not wait for end turn now as there are AI actions first
    AddStep([panelTurn] { return new StepGameEndTurnSimple(panelTurn); });
    // MAKE ENEMY SHOOT
    AddStep([this, playerAI, local]
            {
                auto attacker = static_cast<Unit *>(GetObjectInCell(cellEnemy1));
                auto target = local->GetUnit(indSoldier1);
                return new StepGameMakeEnemyAttack(attacker, target, playerAI);
            });
    // WAIT ENEMY KILLED
    AddStep([this]
        {
                GameObject * enemy = GetObjectInCell(cellEnemy1);
                return new StepGameWaitEnemyKilled(enemy, GetGameMap());
        });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE SOLDIER TO EXPLORE DIAMONDS AND BLOBS
    AddStep([]
            {
                const core::Pointd2D p0(1000, 250);
                return new StepGameSingleInfo(p0, "TUT_GAME_QUICK_SEL_1");
            });
    AddStep([local, game, isoMap]
            {
                const auto unit = local->GetUnit(indSoldier1);
                const core::Pointd2D p0(1300, 350);
                return new StepGameUnit(game, isoMap, unit, p0);
            });
    AddStep([this, local, isoMap, game]
            {
                const auto unit = local->GetUnit(indSoldier1);
                const Cell2D target(5, 42);
                const core::Pointd2D p0(1150, 500);
                return new StepGameMoveUnitSimple(game, unit, isoMap, target, p0);
            });
    AddStep([] { return new StepDelay(0.5f); });

    // SELECT SOLDIER 1 AND COLLECT BLOBS
    AddStep([hud]
            {
                const sgl::core::Pointd2D p0(100, 600);
                return new StepGameQuickUnitButton(hud, indSoldier1, nullptr, p0);
            });
    AddStep([isoMap, gameMap, local, game]
            {
                const auto unit = local->GetUnit(indSoldier1);
                const Cell2D areaTL(2, 46);
                const Cell2D areaBR(4, 48);
                const core::Pointd2D p0(500, 150);

                return new StepGameCollectObjects(unit, ObjectData::TYPE_BLOBS, game, gameMap, isoMap,
                                                  areaTL, areaBR, "TUT_GAME_COLLECT_BLOBS", p0);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // END TURN
    AddStep([panelTurn] { return new StepGameEndTurnSimple(panelTurn); });
    AddStep([gs] { return new StepGameWaitTurn(gs); });
    // SAVE GAME
    AddStep([game, gs] { return new StepSaveGame(game, gs); });
    AddStep([] { return new StepDelay(0.5f); });
    // COLLECT DIAMONDS WITH SOLDIER 1
    AddStep([isoMap, gameMap, local, game]
            {
                const auto unit = local->GetUnit(indSoldier1);
                const Cell2D areaTL(7, 41);
                const Cell2D areaBR(9, 42);
                const core::Pointd2D p0(500, 150);

                return new StepGameCollectObjects(unit, ObjectData::TYPE_DIAMONDS, game, gameMap, isoMap,
                                                  areaTL, areaBR, "TUT_GAME_COLLECT_DIAMONDS", p0);
            });

    // MOVE VIEW BACK TO BASE
    AddStep([panelTurn]
            {
                const core::Pointd2D p0(800, 500);
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
            const int goal = 0;
            return new StepGameSecondaryMissionGoal(hud, goal);
        });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
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
                const auto unit = local->GetUnit(indWorker2);
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
                return new StepGameDialogTrading(hud);
            });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    AddStep([] { return new StepDelay(0.5f); });
    // BUILD SOLDIER 2
    AddStep([]
            {
                const core::Pointd2D p0(600, 150);
                return new StepGameSingleInfo(p0, "TUT_GAME_BUILD_SOLDIER_1");
            });
    AddStep([this, game, isoMap]
            {
                const core::Pointd2D p0(500, 200);
                const GameObject * barracks = GetObjectInCell(cellBarracks);
                return new StepGameSelectObject(game, isoMap, barracks, "TUT_GAME_BARRACKS_1", p0);
            });
    AddStep([game, gs, panelActions]
            {
                return new StepGameBuildUnitStart(game, gs, panelActions,
                                                  PanelObjectActions::BTN_BUILD_UNIT_BARRACKS);
            });
    AddStep([hud] { return new StepGameBuildUnitEnd(hud); });
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    AddStep([this]
            {
                const auto barracks = static_cast<Structure *>(GetObjectInCell(cellBarracks));
                return new StepDelay(barracks->GetTimeBuildUnit());
            });
    AddStep([] { return new StepDelay(0.5f); });
    // ADD ENEMIES
    AddStep([this, playerAI]
            {
                return new StepGameAddEnemy(GetGameMap(), playerAI, ObjectData::TYPE_UNIT_SOLDIER1,
                                            cellEnemy2, false);
            });
    AddStep([this, playerAI]
            {
                return new StepGameAddEnemy(GetGameMap(), playerAI, ObjectData::TYPE_UNIT_SCOUT1,
                                            cellEnemy3, false);
            });
    AddStep([this, playerAI]
            {
                return new StepGameAddEnemy(GetGameMap(), playerAI, ObjectData::TYPE_UNIT_SCOUT1,
                                            cellEnemy4, false);
            });
    AddStep([this, playerAI]
            {
                return new StepGameAddEnemy(GetGameMap(), playerAI, ObjectData::TYPE_UNIT_SOLDIER1,
                                            cellEnemy5, false);
            });
    // SET HEALTH OF ENEMIES
    AddStep([this]
            {
                std::vector<GameObject *> enemies;
                enemies.emplace_back(GetObjectInCell(cellEnemy2));
                enemies.emplace_back(GetObjectInCell(cellEnemy3));
                enemies.emplace_back(GetObjectInCell(cellEnemy4));
                enemies.emplace_back(GetObjectInCell(cellEnemy5));

                return new StepGameSetObjectHealth(std::move(enemies), healthEnemyDef);
            });
    // SET ENERGY OF ENEMIES
    AddStep([this]
            {
                std::vector<GameObject *> enemies;
                enemies.emplace_back(GetObjectInCell(cellEnemy2));
                enemies.emplace_back(GetObjectInCell(cellEnemy3));
                enemies.emplace_back(GetObjectInCell(cellEnemy4));
                enemies.emplace_back(GetObjectInCell(cellEnemy5));

                return new StepGameSetObjectEnergy(std::move(enemies), energyEnemyDef);
            });
    // DISABLE FATAL HIT FOR ENEMIES
    AddStep([this]
            {
                std::vector<GameObject *> enemies;
                enemies.emplace_back(GetObjectInCell(cellEnemy2));
                enemies.emplace_back(GetObjectInCell(cellEnemy3));
                enemies.emplace_back(GetObjectInCell(cellEnemy4));
                enemies.emplace_back(GetObjectInCell(cellEnemy5));

                return new StepGameSetObjectFatalHit(std::move(enemies), false);
            });
    // RE-ENABLE AI
    AddStep([playerAI] { return new StepAISetActive(playerAI->GetAI(), true); });
    // SELECT SOLDIER 2
    AddStep([local, game, isoMap]
            {
                const auto unit = local->GetUnit(indSoldier2);
                const core::Pointd2D p0(1300, 450);
                return new StepGameUnit(game, isoMap, unit, p0);
            });
    // MOVE CAMERA
    AddStep([] { return new StepDelay(0.5f); });
    AddStep([game]
            {
                const int movX = 0;
                const int movY = 300;
                return new StepGameMoveCamera(movX, movY);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // MOVE SOLDIER 2 NEAR WALL
    AddStep([this, local, isoMap, game]
            {
                const auto unit = local->GetUnit(indSoldier2);
                const Cell2D target(22, 23);
                const core::Pointd2D p0(200, 600);
                return new StepGameMoveUnitSimple(game, unit, isoMap, target, p0);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // ANNOUNCE FIGHT
    AddStep([]
            {
                const core::Pointd2D p0(600, 200);
                return new StepGameSingleInfo(p0, "TUT_GAME_KILL_ENEMIES");
            });
    // RE-ENABLE CAMERA
    AddStep([this] { return new StepGameEnableCamera(GetCameraMapController()); });
    // WAIT FOR PLAYER TO KILL ALL ENEMIES
    AddStep([this]
            {
                std::vector<const GameObject *> enemies;
                enemies.emplace_back(GetObjectInCell(cellEnemy2));
                enemies.emplace_back(GetObjectInCell(cellEnemy3));
                enemies.emplace_back(GetObjectInCell(cellEnemy4));
                enemies.emplace_back(GetObjectInCell(cellEnemy5));

                return new StepGameWaitEnemiesKilled(std::move(enemies), GetGameMap());
            });
    // DISABLE CAMERA AGAIN
    AddStep([this] { return new StepGameDisableCamera(GetCameraMapController()); });
    // MOVE VIEW BACK TO BASE
    AddStep([panelTurn]
            {
                const core::Pointd2D p0(800, 300);
                return new StepGameBackToBase(panelTurn, "TUT_GAME_BACK_TO_BASE_1c", p0);
            });
    AddStep([] { return new StepDelay(0.5f); });
    // COLLECT PRIMARY MISSION GOAL
    AddStep([localBase, game, isoMap]
            {
                const core::Pointd2D p0(500, 200);
                return new StepGameSelectObject(game, isoMap, localBase, "TUT_GAME_BASE_4", p0);
            });
    AddStep([game, gs, panelActions] { return new StepGameMissionGoalsIcon(game, gs, panelActions, false); });
    AddStep([hud] { return new StepGamePrimaryMissionGoal(hud); });
    */
}

TutorialGame2::~TutorialGame2()
{
    auto game = GetScreen()->GetGame();

    // reset flags for unit soldier 1
    const Player * local = game->GetActivePlayerByIndex(0);
    auto unit1 = local->GetUnit(indSoldier1);

    if(unit1 != nullptr)
    {
        unit1->SetPerfectShot(false);
        unit1->SetFatalHit(true);
    }

    // reset flags for tower 1
    auto tower1 = GetObjectInCell(cellTower1);

    if(tower1 != nullptr)
        tower1->SetPerfectShot(false);

    // make sure trees are no longer indestructible
    std::vector<GameObject *> trees;
    GetGameMap()->GetObjectsInAreaByType(trees, areaTreesTL, areaTreesBR, ObjectData::TYPE_TREES3);

    for(GameObject * obj : trees)
        obj->SetIndestructible(false);
}

} // namespace game
