#include "Particles/UpdaterSingleLaser.h"

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

    const float minDist = 0.5f;

    while(it != mActiveParticles.end())
    {
        auto p = static_cast<ParticleSingleLaser *>(*it);

        const float vel = p->GetVelocity();
        const float x = p->GetXf() + (vel * delta);
        const float y = p->GetYf() + (vel * delta);
        p->SetPosition(x, y);

        const float diffX = std::fabs(x - p->GetTargetX());

        if(diffX > minDist)
        {
            ++it;
            continue;
        }

        const float diffY = std::fabs(y - p->GetTargetY());

        if(diffY > minDist)
        {
            ++it;
            continue;
        }

        // reached target -> particle can be removed
        it = mActiveParticles.erase(it);

        mParticles.push_back(p);
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
    auto data = static_cast<const ParticleDataSingleLaser &>(initData);
    p->SetTexture(data.tex);
    p->SetPosition(data.x0, data.y0);
    p->SetRotation(data.angle);
    p->SetTarget(data.targetX, data.targetY);
    p->SetVelocity(data.velocity);

    return p;
}

} // namespace game
