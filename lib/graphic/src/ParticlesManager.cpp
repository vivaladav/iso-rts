#include "graphic/ParticlesManager.h"

#include "graphic/ParticlesUpdater.h"

namespace lib
{
namespace graphic
{

ParticlesManager::~ParticlesManager()
{
    for(auto it : mUpdaters)
        delete it.second;
}

void ParticlesManager::Update(float delta)
{
    for(auto it : mUpdaters)
        it.second->Update(delta);
}

void ParticlesManager::Render()
{
    for(auto it : mUpdaters)
        it.second->Render();
}

} // namespace graphic
} // namespace lib
