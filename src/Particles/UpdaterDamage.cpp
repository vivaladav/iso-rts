#include "Particles/UpdaterDamage.h"

#include "Particles/DataParticleDamage.h"
#include "Particles/ParticleDamage.h"

namespace game
{

lib::graphic::Particle * UpdaterDamage::CreateParticle(const lib::graphic::ParticleData & initData)
{
    ParticleDamage * p = nullptr;

    if(mParticles.empty())
        p = new ParticleDamage;
    else
    {
        p = static_cast<ParticleDamage *>(mParticles.back());
        mParticles.pop_back();
    }

    // init data
    auto data = static_cast<const DataParticleDamage &>(initData);
    p->SetData(data);

    return p;
}

} // namespace game
