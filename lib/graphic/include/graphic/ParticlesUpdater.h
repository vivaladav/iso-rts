#pragma once

#include <vector>

namespace lib
{
namespace graphic
{

class Particle;

class ParticlesUpdater
{
public:
    virtual ~ParticlesUpdater();

    void AddParticle(Particle * p);

    virtual void Update(float delta) = 0;

private:
    virtual void OnParticleAdded() = 0;

private:
    std::vector<Particle *> mParticles;
};

// ==================== INLINE FUNCTIONS ====================

inline ParticlesUpdater::~ParticlesUpdater() { }

inline void ParticlesUpdater::AddParticle(Particle * p)
{
    mParticles.push_back(p);

    OnParticleAdded();
}

} // namespace graphic
} // namespace lib
