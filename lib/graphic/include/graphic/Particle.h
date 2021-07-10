#pragma once

#include "TexturedRenderable.h"

namespace lib
{
namespace graphic
{

class Particle : public TexturedRenderable
{
public:
    Particle() = default;
    Particle(Texture * tex);

    bool IsActive() const;
    void SetActive(bool val);

    virtual void Update(float delta) = 0;

    bool IsDone() const;
    virtual void OnDone() = 0;

protected:
    void SetDone();

private:
    bool mActive = true;
    bool mDone = false;
};

// ==================== INLINE FUNCTIONS ====================

inline Particle::Particle(Texture * tex)
    : TexturedRenderable(tex)
{
}

inline bool Particle::IsActive() const { return mActive; }
inline void Particle::SetActive(bool val) { mActive = val; }

inline bool Particle::IsDone() const { return mDone; }

inline void Particle::SetDone()
{
    mDone = true;
    OnDone();
}

} // namespace graphic
} // namespace lib
