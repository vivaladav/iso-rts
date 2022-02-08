#pragma once

#include <sgl/graphic/ParticleData.h>

namespace game
{

class GameMap;
class GameObject;

struct DataParticleSingleLaser : public lib::graphic::ParticleData
{
    DataParticleSingleLaser(lib::graphic::Texture * t, GameMap * gm, GameObject * obj, double ang,
                            float x, float y, float tx, float ty, float sp, float dmg)
        : ParticleData(t)
        , map(gm)
        , target(obj)
        , angle(ang)
        , x0(x)
        , y0(y)
        , targetX(tx)
        , targetY(ty)
        , speed(sp)
        , damage(dmg)
    {
    }

    GameMap * map = nullptr;
    GameObject * target = nullptr;
    double angle = 0;
    float x0 = 0.f;
    float y0 = 0.f;
    float targetX = 0.f;
    float targetY = 0.f;
    float speed = 0.f;
    float damage = 0.f;
};

} // namespace game
