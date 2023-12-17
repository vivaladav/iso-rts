#pragma once

#include <sgl/graphic/ParticleData.h>

namespace game
{

struct DataParticleDamage : public sgl::graphic::TexturedParticleData
{
    DataParticleDamage(sgl::graphic::Texture * t, double ang, float x, float y,
                       float vx, float vy, float sp, float ds, float sc, unsigned int col)
        : sgl::graphic::TexturedParticleData(x, y, sp, t)
        , angle(ang)
        , velX(vx)
        , velY(vy)
        , decaySpeed(ds)
        , scale(sc)
        , color(col)
    {
    }

    double angle = 0;   // degs
    float velX = 0.f;   // [-1,1]
    float velY = 0.f;   // [-1,1]
    float decaySpeed = 1.f;
    float scale = 1.f;
    unsigned int color = 0xFFFFFF;
};

} // namespace game
