#pragma once

#include <graphic/ParticlesUpdater.h>

namespace game
{

class UpdaterDamage : public lib::graphic::ParticlesUpdater
{
private:
    lib::graphic::Particle * CreateParticle(const lib::graphic::ParticleData & initData) override;
};

} // namespace game
