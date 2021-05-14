#include "graphic/ParticlesUpdater.h"

#include "graphic/Particle.h"

namespace lib
{
namespace graphic
{

ParticlesUpdater::~ParticlesUpdater() { }

void ParticlesUpdater::Render()
{
    for(auto p : mActiveParticles)
        p->Render();
}

} // namespace graphic
} // namespace lib
