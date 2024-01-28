#include "Particles/UpdaterHealing.h"

#include "Particles/DataParticleHealing.h"
#include "Particles/ParticleHealing.h"

namespace game
{

sgl::graphic::Particle * UpdaterHealing::CreateParticle(const sgl::graphic::ParticleData & initData)
{
    ParticleHealing * p = nullptr;

    if(mParticles.empty())
        p = new ParticleHealing;
    else
    {
        p = static_cast<ParticleHealing *>(mParticles.back());
        mParticles.pop_back();
    }

    // init data
    auto data = static_cast<const DataParticleHealing &>(initData);
    p->SetData(data);

    return p;
}

} // namespace game
