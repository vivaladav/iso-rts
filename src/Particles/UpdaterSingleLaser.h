#pragma once

#include <graphic/ParticlesUpdater.h>

#include <vector>

namespace game
{

struct ParticleDataSingleLaser : public lib::graphic::ParticleData
{
    ParticleDataSingleLaser(lib::graphic::Texture * t, double ang, float x, float y, float tx, float ty, float sp)
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

class UpdaterSingleLaser : public lib::graphic::ParticlesUpdater
{
public:
    ~UpdaterSingleLaser();

    void Update(float delta) override;

private:
    lib::graphic::Particle * CreateParticle(const lib::graphic::ParticleData & initData) override;

private:
    std::vector<lib::graphic::Particle *> mParticles;
};

} // namespace game
