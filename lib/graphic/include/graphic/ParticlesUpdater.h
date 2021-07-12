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

} // namespace graphic
} // namespace lib
