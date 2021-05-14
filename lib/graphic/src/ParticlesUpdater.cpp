#include "graphic/ParticlesUpdater.h"

#include "graphic/Particle.h"

namespace lib
{
namespace graphic
{

void ParticlesUpdater::Render()
{
    for(auto p : mActiveParticles)
        p->Render();
}

} // namespace graphic
} // namespace lib
