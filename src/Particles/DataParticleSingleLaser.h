#pragma once

#include <sgl/graphic/ParticleData.h>

namespace game
{

class GameMap;
class GameObject;

struct DataParticleSingleLaser : public sgl::graphic::TexturedParticleData
{
    DataParticleSingleLaser(sgl::graphic::Texture * t, GameMap * gm, GameObject * obj, double ang,
                            float x, float y, float tx, float ty, float sp, float dmg)
        : sgl::graphic::TexturedParticleData(x, y, sp ,t)
        , map(gm)
        , target(obj)
        , angle(ang)
        , targetX(tx)
        , targetY(ty)
        , damage(dmg)
    {
    }

    GameMap * map = nullptr;
    GameObject * target = nullptr;
    double angle = 0;
    float targetX = 0.f;
    float targetY = 0.f;
    float damage = 0.f;
};

} // namespace game
