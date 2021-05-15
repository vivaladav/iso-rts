#pragma once

#include <graphic/ParticleData.h>

namespace game
{

struct DataParticleSingleLaser : public lib::graphic::ParticleData
{
    DataParticleSingleLaser(lib::graphic::Texture * t, double ang, float x, float y, float tx, float ty, float sp)
        : ParticleData(t)
        , angle(ang)
        , x0(x)
        , y0(y)
        , targetX(tx)
        , targetY(ty)
        , speed(sp)
    {
    }

    double angle = 0;
    float x0 = 0.f;
    float y0 = 0.f;
    float targetX = 0.f;
    float targetY = 0.f;
    float speed = 0.f;
};

} // namespace game
