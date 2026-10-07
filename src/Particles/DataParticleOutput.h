#pragma once

#include <sgl/graphic/ParticleData.h>

namespace game
{

enum OutputType : unsigned int
{
    OT_BLOBS,
    OT_DIAMONDS,
    OT_ENERGY,
    OT_MATERIAL,
    OT_MONEY,
    OT_RESEARCH,

    NUM_OUTPUT_TYPES,

    OT_NULL
};

struct DataParticleOutput : public sgl::graphic::ParticleData
{
    DataParticleOutput(int q, OutputType t, float x, float y, float de)
        : sgl::graphic::ParticleData(x, y, 50.f, de)
        , quantity(q)
        , type(t)
    {
    }

    DataParticleOutput(int q, OutputType t, float x, float y, float sp, float de, float ds, float tl)
        : sgl::graphic::ParticleData(x, y, sp, de)
        , quantity(q)
        , type(t)
        , decaySpeed(ds)
        , timeLife(tl)
    {
    }

    int quantity = 0;
    OutputType type;
    float decaySpeed = 70.f;
    float timeLife = 1.5f;
};

} // namespace game
