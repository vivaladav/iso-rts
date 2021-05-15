#pragma once

#include <graphic/ParticlesUpdater.h>

#include <vector>

namespace game
{

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
