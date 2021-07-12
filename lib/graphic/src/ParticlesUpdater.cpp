#include "graphic/ParticlesUpdater.h"

#include "graphic/Particle.h"

namespace lib
{
namespace graphic
{

ParticlesUpdater::~ParticlesUpdater()
{
    for(auto p : mParticles)
        delete p;
}

void ParticlesUpdater::AddParticle(const ParticleData & initData)
{
    Particle * p = CreateParticle(initData);
    p->ClearDone();

    mActiveParticles.push_back(p);
}

void ParticlesUpdater::Update(float delta)
{
    auto it = mActiveParticles.begin();

    while(it != mActiveParticles.end())
    {
        auto p = *it;

        p->Update(delta);

        // particle can be removed
        if(p->IsDone())
        {
            it = mActiveParticles.erase(it);
            mParticles.push_back(p);
        }
        else
            ++it;
    }
}

void ParticlesUpdater::Render()
{
    for(auto p : mActiveParticles)
        p->Render();
}

} // namespace graphic
} // namespace lib
