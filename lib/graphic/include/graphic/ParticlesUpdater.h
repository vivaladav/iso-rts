#pragma once

#include <vector>

namespace lib
{
namespace graphic
{

class Particle;
class Texture;

struct ParticleData
{
    ParticleData(Texture * t) : tex(t) {}

    Texture * tex = nullptr;
};

class ParticlesUpdater
{
public:
    virtual ~ParticlesUpdater();

    void AddParticle(const ParticleData & initData);

    virtual void Update(float delta) = 0;

    void Render();

private:
    virtual Particle * CreateParticle(const ParticleData & initData) = 0;

protected:
    std::vector<Particle *> mActiveParticles;
};

// ==================== INLINE FUNCTIONS ====================

inline void ParticlesUpdater::AddParticle(const ParticleData & initData)
{
    Particle * p = CreateParticle(initData);

    mActiveParticles.push_back(p);
}

} // namespace graphic
} // namespace lib
