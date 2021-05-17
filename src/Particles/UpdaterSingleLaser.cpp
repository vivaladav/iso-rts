#include "Particles/UpdaterSingleLaser.h"

#include "Particles/DataParticleSingleLaser.h"
#include "Particles/ParticleSingleLaser.h"

#include <cmath>

namespace game
{

UpdaterSingleLaser::~UpdaterSingleLaser()
{
    for(auto p : mParticles)
        delete p;
}

void UpdaterSingleLaser::Update(float delta)
{
    auto it = mActiveParticles.begin();

    while(it != mActiveParticles.end())
    {
        auto p = static_cast<ParticleSingleLaser *>(*it);

        p->Move(delta);

        // reached target -> particle can be removed
        if(p->IsDone())
        {
            p->OnDone();

            it = mActiveParticles.erase(it);
            mParticles.push_back(p);
        }
        else
            ++it;
    }
}

lib::graphic::Particle * UpdaterSingleLaser::CreateParticle(const lib::graphic::ParticleData & initData)
{
    using namespace lib::graphic;

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
