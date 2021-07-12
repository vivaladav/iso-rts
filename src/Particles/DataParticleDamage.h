#pragma once

#include <graphic/ParticleData.h>

namespace game
{

struct DataParticleDamage : public lib::graphic::ParticleData
{
    DataParticleDamage(lib::graphic::Texture * t, double ang, float x, float y,
                       float vx, float vy, float sp, float ds, unsigned int col)
        : ParticleData(t)
        , angle(ang)
        , x0(x)
        , y0(y)
        , velX(vx)
        , velY(vy)
        , speed(sp)
        , decaySpeed(ds)
        , color(col)
    {
    }

    double angle = 0;   // degs
    float x0 = 0.f;
    float y0 = 0.f;
    float velX = 0.f;   // [-1,1]
    float velY = 0.f;   // [-1,1]
    float speed = 0.f;
    float decaySpeed = 1.f;
    unsigned int color = 0xFFFFFF;
};

} // namespace game
