#pragma once

#include <core/Application.h>

#include <string>
#include <vector>

namespace lib
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

class Player;

enum StateId : int;

enum Difficulty : unsigned int
{
    EASY,
    MEDIUM,
    HARD,

    NUM_DIFFICULTIES
};

class Game : public lib::core::Application
{
public:
    Game(int argc, char * argv[]);
    ~Game();

    const std::vector<std::string> & GetMapFiles() const;
    const std::string & GetCurrentMapFile() const;
    void SetCurrentMap(unsigned int index);

    void SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    void RequestNextActiveState(StateId sid);

    Difficulty GetDifficulty() const;
    void SetDifficulty(Difficulty level);

    // -- players --
    void AddPlayer(const char * name, int pid);
    void ClearPlayers();

    int GetNumPlayers() const;

    Player * GetPlayer(unsigned int index) const;

private:
    void Update(float delta) override;

private:
    std::vector<std::string> mMapFiles;
    std::vector<Player *> mPlayers;
    unsigned int mCurrMap = 0;

    lib::graphic::Renderer * mRenderer = nullptr;
    lib::graphic::Window * mWin = nullptr;

    lib::utilities::StateManager * mStateMan = nullptr;

    lib::sgui::Stage * mStage = nullptr;
    lib::graphic::Font * mFontGui = nullptr;

    Difficulty mDiff = EASY;

    unsigned char mClearR = 0;
    unsigned char mClearG = 0;
    unsigned char mClearB = 0;
    unsigned char mClearA = 255;
};

inline const std::vector<std::string> & Game::GetMapFiles() const { return mMapFiles; }
inline const std::string & Game::GetCurrentMapFile() const { return mMapFiles[mCurrMap]; }
inline void Game::SetCurrentMap(unsigned int index)
{
    if(index < mMapFiles.size())
        mCurrMap = index;
}

inline void Game::SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mClearR = r;
    mClearG = g;
    mClearB = b;
    mClearA = a;
}

inline Difficulty Game::GetDifficulty() const { return mDiff; }
inline void Game::SetDifficulty(Difficulty level) { mDiff = level; }

inline int Game::GetNumPlayers() const { return mPlayers.size(); }

inline Player * Game::GetPlayer(unsigned int index) const
{
    if(index < mPlayers.size())
        return mPlayers[index];
    else
        return nullptr;
}

} // namespace game
