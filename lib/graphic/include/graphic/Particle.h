#pragma once

#include "TexturedRenderable.h"

namespace lib
{
namespace graphic
{

class Particle : public TexturedRenderable
{
public:
    Particle(Texture * tex);

    bool IsActive() const;
    void SetActive(bool val);

private:
    bool mActive = true;
};

// ==================== INLINE FUNCTIONS ====================

inline Particle::Particle(Texture * tex)
    : TexturedRenderable(tex)
{
}

inline bool Particle::IsActive() const { return mActive; }
inline void Particle::SetActive(bool val) { mActive = val; }


} // namespace graphic
} // namespace lib
