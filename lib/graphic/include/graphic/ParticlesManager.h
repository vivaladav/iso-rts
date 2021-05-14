#pragma once

#include <unordered_map>

namespace lib
{
namespace graphic
{

class ParticlesUpdater;

class ParticlesManager
{
public:
    ~ParticlesManager();

    void RegisterUpdater(unsigned int key, ParticlesUpdater * updater);
    void UnregisterUpdater(unsigned int key);

    void Update(float delta);
    void Render();

private:
    std::unordered_map<unsigned int, ParticlesUpdater *> mUpdaters;
};

inline void ParticlesManager::RegisterUpdater(unsigned int key, ParticlesUpdater * updater)
{
    mUpdaters.emplace(key, updater);
}

inline void ParticlesManager::UnregisterUpdater(unsigned int key)
{
    mUpdaters.erase(key);
}

} // namespace graphic
} // namespace lib
