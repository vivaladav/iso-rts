#pragma once

#include <sgl/graphic/TexturedParticle.h>

namespace game
{

class GameMap;
class GameObject;

struct DataParticleDamage;

class ParticleDamage : public sgl::graphic::TexturedParticle
{
public:
    void SetData(const DataParticleDamage & data);

    float GetSpeed() const;

    void Update(float delta) override;

private:
    void SetScale(float scale);
    void SetStart(int x0, int y0);

private:
    float mPosXf = 0.f;
    float mPosYf = 0.f;

    float mSpeed = 0.f;
    float mVelX = 0.f;
    float mVelY = 0.f;

    float mAlpha = 255.f;
    float mDecaySpeed = 1.f;
};

inline float ParticleDamage::GetSpeed() const { return mSpeed; }


} // namespace game
