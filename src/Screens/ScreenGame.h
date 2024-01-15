#pragma once

#include "Cell2D.h"
#include "Screen.h"
#include "GameObjects/GameObjectAction.h"
#include "GameObjects/GameObjectTypes.h"

#include <chrono>
#include <functional>
#include <unordered_map>
#include <vector>

namespace sgl
{
    namespace ai { class Pathfinder; }
    namespace graphic
    {
        class ParticlesManager;
        class ParticlesUpdater;
    }
}

namespace game
{

class AttackRangeIndicator;
class CameraMapController;
class CellProgressBar;
class ConquestIndicator;
class GameHUD;
class GameMap;
class GameObject;
class IsoLayer;
class IsoMap;
class MiniMap;
class MoveIndicator;
class Player;
class PlayerAI;
class StructureIndicator;
class Unit;
class WallIndicator;

enum MissionType : unsigned int;
enum PlayerFaction : unsigned int;

enum ParticlesUpdaterId : unsigned int
{
    PU_DAMAGE,
    PU_LOOTBOX_PRIZE,
    PU_SINGLE_LASER
};

class ScreenGame : public Screen
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    unsigned int GetPlayTimeInSec() const;

    void Update(float delta) override;
    void Render() override;

    void OnKeyDown(sgl::core::KeyboardEvent & event) override;
    void OnKeyUp(sgl::core::KeyboardEvent & event) override;
    void OnMouseButtonUp(sgl::core::MouseButtonEvent & event) override;
    void OnMouseMotion(sgl::core::MouseMotionEvent & event) override;
    void OnWindowExposed(sgl::graphic::WindowEvent & event) override;
    void OnWindowHidden(sgl::graphic::WindowEvent & event) override;
    void OnWindowMouseEntered(sgl::graphic::WindowEvent & event) override;
    void OnWindowMouseLeft(sgl::graphic::WindowEvent & event) override;

    void CancelProgressBar(CellProgressBar * pb);
    CellProgressBar * CreateProgressBar(const Cell2D & cell, float time, PlayerFaction faction);

    void ClearObjectAction(GameObject * obj);
    void SetObjectActionCompleted(GameObject * obj);

    sgl::graphic::ParticlesUpdater * GetParticleUpdater(ParticlesUpdaterId updaterId);

    void ClearSelection(Player * player);
    void SelectObject(GameObject * obj, Player * player);

    void CenterCameraOverCell(int row, int col);
    void CenterCameraOverObject(GameObject * obj);

    MiniMap * GetMiniMap() const;
    void SetMiniMapEnabled(bool val);

    bool GetPaused() const;
    void SetPause(bool paused);

private:
    void OnApplicationQuit(sgl::core::ApplicationEvent & event) override;

    void InitMusic();
    void InitParticlesSystem();

    void CreateIsoMap();
    void CreateLayers();

    void CreateUI();

    void LoadMapFile();

    void UpdateProgressBars(float delta);

    void UpdateAI(float delta);
    void ExecuteAIAction(PlayerAI * ai);

    void UpdateGameEnd();
    void HandleGameOver();
    void HandleGameWon();
    void AssignMapToFaction(PlayerFaction faction);
    void AssignWinningResources(Player * player);
    bool CheckGameOverForLocalPlayer();

    int CellToIndex(const Cell2D & cell) const;

    bool SetupNewUnit(GameObjectTypeId type, GameObject * gen, Player * player, const std::function<void()> & OnDone = []{});
    bool SetupStructureConquest(Unit * unit, const Cell2D & start, const Cell2D & end, Player * player,
                                const std::function<void()> & OnDone = []{});
    bool SetupStructureBuilding(Unit * unit, const Cell2D & cellTarget, Player * player,
                                const std::function<void()> & OnDone = []{});
    bool SetupUnitUpgrade(GameObject * obj, Player * player, const std::function<void()> & OnDone = []{});
    bool SetupUnitMove(Unit * unit, const Cell2D & start, const Cell2D & end,
                       const std::function<void()> & OnDone = []{});
    bool SetupConnectCells(Unit * unit, const std::function<void()> & OnDone = []{});

    void HandleUnitOnMouseMove(Unit * unit, const Cell2D & cell);
    void HandleUnitMoveOnMouseMove(Unit * unit, const Cell2D & currCell);
    void HandleUnitConquestOnMouseMove(Unit * unit, const Cell2D & currCell);
    void HandleUnitBuildWallOnMouseMove(Unit * unit, const Cell2D & currCell);
    void HandleUnitBuildStructureOnMouseMove(Unit * unit, const Cell2D & currCell);

    void HandleUnitMoveOnMouseUp(Unit * unit, const Cell2D & clickCell);
    void HandleUnitBuildStructureOnMouseUp(Unit * unit, const Cell2D & clickCell);
    void HandleUnitBuildWallOnMouseUp(Unit * unit, const Cell2D & clickCell);

    void HandleSelectionClick(sgl::core::MouseButtonEvent & event);
    void HandleActionClick(sgl::core::MouseButtonEvent & event);

    void StartUnitBuildWall(Unit * unit);

    void ShowAttackIndicators(const GameObject * obj, int range);
    void ClearCellOverlays();

    void CenterCameraOverPlayerBase();

private:
    friend class GameHUD;

    std::vector<Player *> mAiPlayers;

    std::vector<CellProgressBar *> mProgressBars;

    std::vector<ConquestIndicator *> mConquestIndicators;
    std::unordered_map<GameObjectTypeId, StructureIndicator *> mStructIndicators;
    std::vector<WallIndicator *> mWallIndicators;
    std::vector<AttackRangeIndicator *> mAttIndicators;
    StructureIndicator * mTempStructIndicator = nullptr;

    std::vector<unsigned int> mConquestPath;
    std::vector<unsigned int> mWallPath;

    std::vector<GameObjectAction> mActiveObjActions;

    CameraMapController * mCamController = nullptr;

    unsigned int mIdOnSettingsChanged = 0;

    sgl::graphic::ParticlesManager * mPartMan = nullptr;

    std::chrono::time_point<std::chrono::steady_clock> mTimeStart;

    // UI
    GameHUD * mHUD = nullptr;

    GameMap * mGameMap = nullptr;
    IsoMap * mIsoMap = nullptr;

    sgl::ai::Pathfinder * mPathfinder = nullptr;

    MoveIndicator * mMoveInd = nullptr;

    struct Cell2D mPrevCell;

    float mTimerEnergy;
    float mTimerAI;

    float mTimePlayed = 0.f;

    unsigned int mCurrPlayerAI = 0;

    MissionType mMissionType;
    unsigned int mMissionTime = 0;

    bool mPaused = false;
};

inline bool ScreenGame::GetPaused() const { return mPaused; }
inline void ScreenGame::SetPause(bool paused) { mPaused = paused; }

} // namespace game
