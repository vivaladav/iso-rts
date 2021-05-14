#pragma once

#include <graphic/Particle.h>

namespace game
{

class ParticleSingleLaser : public lib::graphic::Particle
{
public:
    ParticleSingleLaser();

    float GetXf() const;
    float GetYf() const;
    void SetPositionf(float x, float y);

    float GetTargetX() const;
    float GetTargetY() const;
    void SetTarget(float x, float y);

    float GetVelocity() const;
    void SetVelocity(float vel);

private:
    float mPosXf = 0.f;
    float mPosYf = 0.f;

    float mTargetX = 0.f;
    float mTargetY = 0.f;

    float mVelocity = 0.f;
};

inline float ParticleSingleLaser::GetXf() const { return mPosXf; }
inline float ParticleSingleLaser::GetYf() const { return mPosYf; }
inline void ParticleSingleLaser::SetPositionf(float x, float y)
{
    mPosXf = x;
    mPosYf = y;

    SetPosition(static_cast<int>(mPosXf), static_cast<int>(mPosYf));
}

inline float ParticleSingleLaser::GetTargetX() const { return mTargetX; }
inline float ParticleSingleLaser::GetTargetY() const { return mTargetY; }

inline void ParticleSingleLaser::SetTarget(float x, float y)
{
    mTargetX = x;
    mTargetY = y;
}

inline float ParticleSingleLaser::GetVelocity() const { return mVelocity; }
inline void ParticleSingleLaser::SetVelocity(float vel) { mVelocity = vel; }

} // namespace game
