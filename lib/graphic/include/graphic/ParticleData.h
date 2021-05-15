#pragma once

namespace lib
{
namespace graphic
{

class Texture;

struct ParticleData
{
    ParticleData(Texture * t) : tex(t) {}

    Texture * tex = nullptr;
};

} // namespace graphic
} // namespace lib
