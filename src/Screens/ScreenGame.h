#pragma once

#include "Cell2D.h"
#include "GameConstants.h"
#include "Screen.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace game
{

class CellProgressBar;
class GameMap;
class IsoLayer;
class IsoMap;
class PanelPlayer;
class Player;
class PlayerAI;

class ScreenGame : public Screen
{
public:
    ScreenGame(Game * game);
    ~ScreenGame();

    void Update(float delta) override;
    void Render() override;

    void OnKeyUp(lib::core::KeyboardEvent & event) override;
    void OnMouseButtonUp(lib::core::MouseButtonEvent & event) override;

    void GameOver();
    void GameWon();

    void CancelProgressBar(const Cell2D & cell);

private:
    void CreateIsoMap();
    void CreateLayers();

    CellProgressBar * CreateProgressBar(const Cell2D & cell, float time, int playerId);
    void UpdateProgressBars(float delta);

    void ClearSelection(Player * player);
    void SelectCell(const Cell2D & cell, Player * player);

    void ShowMoveTargets(const Cell2D & cell, Player * player);

    void UpdateAI(float delta);
    void ExecuteAIAction(PlayerAI * ai);

    int CellToIndex(const Cell2D & cell) const;

    bool SetupCellConquest(const Cell2D & cell, Player * player);
    bool SetupNewUnit(const Cell2D & cell, Player * player);
    bool SetupResourceGeneratorConquest(const Cell2D & start, const Cell2D & end, Player * player);
    bool SetupUnitDestroy(const Cell2D & cell, Player * player);
    bool SetupUnitUpgrade(const Cell2D & cell, Player * player);

private:
    std::vector<Player *> mAiPlayers;

    std::unordered_map<int, CellProgressBar *> mProgressBars;
    std::vector<int> mProgressBarsToDelete;

    PanelPlayer * mPanelPlayer = nullptr;

    GameMap * mGameMap = nullptr;
    IsoMap * mIsoMap = nullptr;

    struct Cell2D mPrevSel;

    float mTimerEnergy = TIME_ENERGY_USE;
    float mTimerAI = TIME_AI_MOVE;

    unsigned int mCurrPlayerAI = 0;

    bool mPaused = false;
};

} // namespace game
