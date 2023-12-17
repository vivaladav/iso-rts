#pragma once

#include <sgl/graphic/ParticlesUpdater.h>

namespace game
{

class UpdaterLootboxPrize : public sgl::graphic::ParticlesUpdater
{
private:
    sgl::graphic::Particle * CreateParticle(const sgl::graphic::ParticleData & initData) override;
};

} // namespace game
