#include "Particles/UpdaterLootboxPrize.h"

#include "Particles/DataParticleLootboxPrize.h"
#include "Particles/ParticleLootboxPrize.h"

namespace game
{

sgl::graphic::Particle * UpdaterLootboxPrize::CreateParticle(const sgl::graphic::ParticleData & initData)
{
    ParticleLootboxPrize * p = nullptr;

    if(mParticles.empty())
        p = new ParticleLootboxPrize;
    else
    {
        p = static_cast<ParticleLootboxPrize *>(mParticles.back());
        mParticles.pop_back();
    }

    // init data
    auto data = static_cast<const DataParticleLootboxPrize &>(initData);
    p->SetData(data);

    return p;
}

} // namespace game
