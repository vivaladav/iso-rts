#pragma once

#include "Cell2D.h"
#include "Screen.h"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace lib
{
    namespace ai { class Pathfinder; }
}

namespace game
{

class CellProgressBar;
class ConquestIndicator;
class GameMap;
class GameObject;
class IsoLayer;
class IsoMap;
class MoveIndicator;
class PanelPlayer;
class PanelResourcesBar;
class Player;
class PlayerAI;
class Unit;

enum PlayerFaction : unsigned int;

class ScreenGame : public Screen
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    void Update(float delta) override;
    void Render() override;

    void OnKeyUp(lib::core::KeyboardEvent & event) override;
    void OnMouseButtonUp(lib::core::MouseButtonEvent & event) override;
    void OnMouseMotion(lib::core::MouseMotionEvent & event) override;

    void GameOver();
    void GameWon();

    void CancelProgressBar(const Cell2D & cell);

    void CreateProgressBar(const Cell2D & cell, float time, Player * player,
                           const std::function<void()> & onCompleted);

private:
    void CreateIsoMap();
    void CreateLayers();

    CellProgressBar * CreateProgressBar(const Cell2D & cell, float time, PlayerFaction playerFaction);
    void UpdateProgressBars(float delta);

    void ClearSelection(Player * player);
    void SelectObject(GameObject * obj, Player * player);

    void UpdateAI(float delta);
    void ExecuteAIAction(PlayerAI * ai);

    int CellToIndex(const Cell2D & cell) const;

    bool SetupNewUnit(GameObject * gen, Player * player);
    bool SetupResourceGeneratorConquest(const Cell2D & start, const Cell2D & end, Player * player);
    bool SetupUnitUpgrade(GameObject * obj, Player * player);
    void SetupUnitMove(Unit * unit, const Cell2D & start, const Cell2D & end,
                       const std::function<void()> & onCompleted = []{});

    void HandleUnitMoveOnMouseMove(Unit * unit, const Cell2D & currCell);
    void HandleUnitConquestOnMouseMove(Unit * unit, const Cell2D & currCell);
    void HandleUnitBuildWallOnMouseMove(Unit * unit, const Cell2D & currCell);

    void HandleUnitMoveOnMouseUp(Unit * unit, const Cell2D clickCell);

    void ClearCellOverlays();

private:
    std::vector<Player *> mAiPlayers;

    std::unordered_map<int, CellProgressBar *> mProgressBars;
    std::vector<int> mProgressBarsToDelete;

    std::vector<ConquestIndicator *> mConquestIndicators;

    // -- UI --
    PanelPlayer * mPanelPlayer = nullptr;
    PanelResourcesBar * mPanelResBar = nullptr;

    GameMap * mGameMap = nullptr;
    IsoMap * mIsoMap = nullptr;

    lib::ai::Pathfinder * mPathfinder = nullptr;

    MoveIndicator * mMoveInd = nullptr;

    struct Cell2D mPrevCell;

    float mTimerEnergy;
    float mTimerAI;

    unsigned int mCurrPlayerAI = 0;

    bool mPaused = false;
};

} // namespace game
