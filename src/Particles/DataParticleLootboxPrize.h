#pragma once

#include <sgl/graphic/ParticleData.h>

namespace game
{

struct DataParticleLootboxPrize : public sgl::graphic::ParticleData
{
    DataParticleLootboxPrize(int q, unsigned int t, float x, float y, float sp, float ds)
        : sgl::graphic::ParticleData(x, y, sp)
        , quantity(q)
        , type(t)
        , decaySpeed(ds)
    {
    }

    int quantity = 0;
    unsigned int type = 0;
    float decaySpeed = 1.f;
};

} // namespace game
