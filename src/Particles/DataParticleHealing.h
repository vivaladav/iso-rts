#pragma once

#include <sgl/graphic/ParticleData.h>

namespace game
{

class GameMap;
class GameObject;

enum PlayerFaction : unsigned int;

struct DataParticleHealing : public sgl::graphic::TexturedParticleData
{
    DataParticleHealing(sgl::graphic::Texture * t, GameMap * gm, GameObject * obj, float x,
                        float y, float tx, float ty, float sp, float hl, PlayerFaction hlr)
        : sgl::graphic::TexturedParticleData(x, y, sp ,t)
        , map(gm)
        , target(obj)
        , targetX(tx)
        , targetY(ty)
        , heal(hl)
        , healer(hlr)
    {
    }

    GameMap * map = nullptr;
    GameObject * target = nullptr;
    PlayerFaction healer;
    float targetX = 0.f;
    float targetY = 0.f;
    float heal = 0.f;
};

} // namespace game
