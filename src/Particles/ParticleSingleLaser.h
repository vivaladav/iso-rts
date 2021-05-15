#pragma once

#include <graphic/Particle.h>

namespace game
{

struct DataParticleSingleLaser;

class ParticleSingleLaser : public lib::graphic::Particle
{
public:
    void SetData(const DataParticleSingleLaser & data);

    float GetXf() const;
    float GetYf() const;

    float GetTargetX() const;
    float GetTargetY() const;

    float GetSpeed() const;

    void Move(float delta);

    bool IsDone() const;

private:
    void SetStartAndTarget(int x0, int y0, int tx, int ty);
    void SetSpeed(float vel);

private:
    float mPosXf = 0.f;
    float mPosYf = 0.f;

    int mTargetX = 0;
    int mTargetY = 0;

    float mSpeed = 0.f;
    float mVelX = 0.f;
    float mVelY = 0.f;

    bool mDoneX = false;
    bool mDoneY = false;
};

inline float ParticleSingleLaser::GetXf() const { return mPosXf; }
inline float ParticleSingleLaser::GetYf() const { return mPosYf; }

inline float ParticleSingleLaser::GetTargetX() const { return mTargetX; }
inline float ParticleSingleLaser::GetTargetY() const { return mTargetY; }

inline float ParticleSingleLaser::GetSpeed() const { return mSpeed; }
inline void ParticleSingleLaser::SetSpeed(float vel) { mSpeed = vel; }

inline bool ParticleSingleLaser::IsDone() const { return mDoneX && mDoneY; }

} // namespace game
