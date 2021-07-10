#pragma once

#include <vector>

namespace lib
{
namespace graphic
{

class Particle;
class Texture;

struct ParticleData;

class ParticlesUpdater
{
public:
    virtual ~ParticlesUpdater();

    void AddParticle(const ParticleData & initData);

    void Update(float delta);

    void Render();

private:
    virtual Particle * CreateParticle(const ParticleData & initData) = 0;

protected:
    std::vector<Particle *> mActiveParticles;
    std::vector<Particle *> mParticles;
};

// ==================== INLINE FUNCTIONS ====================

inline void ParticlesUpdater::AddParticle(const ParticleData & initData)
{
    Particle * p = CreateParticle(initData);

    mActiveParticles.push_back(p);
}

} // namespace graphic
} // namespace lib
