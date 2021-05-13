#include "graphic/ParticlesManager.h"

#include "graphic/ParticlesUpdater.h"

namespace lib
{
namespace graphic
{

void ParticlesManager::Update(float delta)
{
    for(auto it : mUpdaters)
        it.second->Update(delta);
}

void ParticlesManager::AddParticleToUpdater(unsigned int updaterId)
{
    auto res = mUpdaters.find(updaterId);

    if(mUpdaters.end() == res)
        return ;

    // TODO assign particle
}

} // namespace graphic
} // namespace lib
