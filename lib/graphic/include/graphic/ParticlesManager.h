#pragma once

#include <vector>
#include <unordered_map>

namespace lib
{
namespace graphic
{

class Particle;
class ParticlesUpdater;

class ParticlesManager
{
public:
    void Update(float delta);

    void AddParticleToUpdater(unsigned int updaterId);

private:
    std::vector<Particle *> mParticles;

    std::unordered_map<unsigned int, ParticlesUpdater *> mUpdaters;
};

} // namespace graphic
} // namespace lib
