#include "Particles/UpdaterSingleLaser.h"

#include "Particles/DataParticleSingleLaser.h"
#include "Particles/ParticleSingleLaser.h"

namespace game
{

sgl::graphic::Particle * UpdaterSingleLaser::CreateParticle(const sgl::graphic::ParticleData & initData)
{
    ParticleSingleLaser * p = nullptr;

    if(mParticles.empty())
        p = new ParticleSingleLaser;
    else
    {
        p = static_cast<ParticleSingleLaser *>(mParticles.back());
        mParticles.pop_back();
    }

    // init data
    auto data = static_cast<const DataParticleSingleLaser &>(initData);
    p->SetData(data);

    return p;
}

} // namespace game
