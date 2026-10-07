#pragma once

#include <sgl/graphic/ParticleData.h>

namespace game
{

struct DataParticleHitPoints : public sgl::graphic::ParticleData
{
    DataParticleHitPoints(int v, float x, float y, float sp, float de, float ds, float md, bool f)
        : sgl::graphic::ParticleData(x, y, sp, de)
        , value(v)
        , decaySpeed(ds)
        , maxDistance(md)
        , fatal(f)
    {
    }

    DataParticleHitPoints(float x, float y, float sp, float de, float ds, float md)
        : sgl::graphic::ParticleData(x, y, sp, de)
        , decaySpeed(ds)
        , maxDistance(md)
        , miss(true)
    {
    }

    int value = 0;
    float decaySpeed = 1.f;
    float maxDistance = 10.f;
    bool fatal = false;
    bool miss = false;
};

} // namespace game
