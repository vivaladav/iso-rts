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

void ParticlesUpdater::Update(float delta)
{
    auto it = mActiveParticles.begin();

    while(it != mActiveParticles.end())
    {
        auto p = *it;

        p->Update(delta);

        // reached target -> particle can be removed
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
