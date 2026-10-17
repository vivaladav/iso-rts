#pragma once

namespace sgl
{
    namespace graphic { class ParticlesManager; }
}

namespace game
{

class Game;
class GameMap;
class Player;

class ObjectInitData
{
public:
    ObjectInitData(const Game * game, GameMap * map,
                   const sgl::graphic::ParticlesManager * pm,
                   Player * p);

    const Game * GetGame() const;
    GameMap * GetGameMap() const;
    const sgl::graphic::ParticlesManager * GetParticlesManager() const;
    Player * GetPlayer() const;

private:
    const Game * mGame;
    GameMap * mGameMap = nullptr;
    const sgl::graphic::ParticlesManager * mPartMan;
    Player * mPlayer = nullptr;
};

inline ObjectInitData::ObjectInitData(const Game * game, GameMap * map,
                                      const sgl::graphic::ParticlesManager * pm,
                                      Player * p)
    : mGame(game)
    , mGameMap(map)
    , mPartMan(pm)
    , mPlayer(p)
{
}

inline const Game * ObjectInitData::GetGame() const { return mGame; }

inline GameMap * ObjectInitData::GetGameMap() const { return mGameMap; }

inline const sgl::graphic::ParticlesManager * ObjectInitData::GetParticlesManager() const
{
    return mPartMan;
}

inline Player * ObjectInitData::GetPlayer() const { return mPlayer; }

} // namespace game
