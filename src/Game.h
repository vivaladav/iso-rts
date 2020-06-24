#pragma once

#include <core/Application.h>

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

class Game : public lib::core::Application
{
public:
    Game(int argc, char * argv[]);
    ~Game();

    void SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    void RequestNextActiveState(StateId sid);

    // -- players --
    void AddPlayer(const char * name, int pid);
    void ClearPlayers();

    int GetNumPlayers() const;

    Player * GetPlayer(unsigned int index) const;

private:
    void Update() override;

private:
    std::vector<Player *> mPlayers;

    lib::graphic::Renderer * mRenderer = nullptr;
    lib::graphic::Window * mWin = nullptr;

    lib::utilities::StateManager * mStateMan = nullptr;

    lib::sgui::Stage * mStage = nullptr;
    lib::graphic::Font * mFontGui = nullptr;

    unsigned char mClearR = 0;
    unsigned char mClearG = 0;
    unsigned char mClearB = 0;
    unsigned char mClearA = 255;
};

inline void Game::SetClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    mClearR = r;
    mClearG = g;
    mClearB = b;
    mClearA = a;
}

inline int Game::GetNumPlayers() const { return mPlayers.size(); }

inline Player * Game::GetPlayer(unsigned int index) const
{
    if(index < mPlayers.size())
        return mPlayers[index];
    else
        return nullptr;
}

} // namespace game
