#pragma once

#include <sgl/graphic/ParticlesUpdater.h>

namespace game
{

class UpdaterSingleLaser : public lib::graphic::ParticlesUpdater
{
private:
    lib::graphic::Particle * CreateParticle(const lib::graphic::ParticleData & initData) override;
};

} // namespace game
