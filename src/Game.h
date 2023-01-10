#pragma once

#include <sgl/core/Application.h>

#include <string>
#include <vector>

namespace sgl
{
    namespace graphic
    {
        class Font;
        class Renderer;
        class Window;
    }

    namespace sgui { class Stage; }

    namespace utilities { class StateManager; }
}

namespace game
{

class MapsRegistry;
class ObjectsDataRegistry;
class Player;

enum Planets : unsigned int;
enum PlayerFaction : unsigned int;
enum StateId : int;

enum Difficulty : unsigned int
{
    EASY,
    MEDIUM,
    HARD,

    NUM_DIFFICULTIES
};

class Game : public sgl::core::Application
{
public:
    Game(int argc, char * argv[]);
    ~Game();

    void InitGameData();
    void ClearGameData();

    const std::string & GetCurrentMapFile() const;
    void SetCurrentTerritory(unsigned int territory);
    void SetCurrentPlanet(Planets planet);
    Planets GetCurrentPlanet() const;

    void SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    void RequestNextActiveState(StateId sid);

    Difficulty GetDifficulty() const;
    void SetDifficulty(Difficulty level);

    MapsRegistry * GetMapsRegistry() const;
    const ObjectsDataRegistry * GetObjectsRegistry() const;

    // -- players --
    Player * AddPlayer(const char * name, int pid);
    void ClearPlayers();

    int GetNumPlayers() const;

    Player * GetPlayerByIndex(unsigned int index) const;
    Player * GetPlayerByFaction(PlayerFaction faction) const;
    Player * GetLocalPlayer() const;

    void SetLocalPlayerFaction(PlayerFaction faction);
    PlayerFaction GetLocalPlayerFaction() const;

private:
    void Update(float delta) override;

private:
    std::vector<Player *> mPlayers;

    sgl::graphic::Renderer * mRenderer = nullptr;
    sgl::graphic::Window * mWin = nullptr;

    sgl::utilities::StateManager * mStateMan = nullptr;

    sgl::sgui::Stage * mStage = nullptr;

    MapsRegistry * mMapsReg = nullptr;
    ObjectsDataRegistry * mObjsRegistry = nullptr;

    Difficulty mDiff = EASY;

    PlayerFaction mLocalFaction;

    unsigned int mCurrMap = 0;
    Planets mCurrPlanet;
    unsigned int mCurrTerritory = 0;

    unsigned char mClearR = 0;
    unsigned char mClearG = 0;
    unsigned char mClearB = 0;
    unsigned char mClearA = 255;
};

inline void Game::SetCurrentTerritory(unsigned int territory)
{
    mCurrTerritory = territory;
}

inline void Game::SetCurrentPlanet(Planets planet) { mCurrPlanet = planet; }
inline Planets Game::GetCurrentPlanet() const { return mCurrPlanet; }

inline void Game::SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mClearR = r;
    mClearG = g;
    mClearB = b;
    mClearA = a;
}

inline Difficulty Game::GetDifficulty() const { return mDiff; }
inline void Game::SetDifficulty(Difficulty level) { mDiff = level; }

inline MapsRegistry * Game::GetMapsRegistry() const { return mMapsReg; }
inline const ObjectsDataRegistry * Game::GetObjectsRegistry() const { return mObjsRegistry; }

inline int Game::GetNumPlayers() const { return mPlayers.size(); }

inline Player * Game::GetPlayerByIndex(unsigned int index) const
{
    if(index < mPlayers.size())
        return mPlayers[index];
    else
        return nullptr;
}

inline Player * Game::GetLocalPlayer() const
{
    // NOTE for now local player is always at index 0. This might change in the future
    const int indLocal = 0;
    return mPlayers[indLocal];
}

inline void Game::SetLocalPlayerFaction(PlayerFaction faction)
{
    mLocalFaction = faction;
}

inline PlayerFaction Game::GetLocalPlayerFaction() const
{
    return mLocalFaction;
}

} // namespace game
