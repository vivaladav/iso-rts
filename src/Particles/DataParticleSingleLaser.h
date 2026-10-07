#pragma once

#include <sgl/graphic/ParticleData.h>

namespace game
{

class GameMap;
class GameObject;

enum PlayerFaction : unsigned int;

struct DataParticleSingleLaser : public sgl::graphic::TexturedParticleData
{
    DataParticleSingleLaser(sgl::graphic::Texture * t, const GameMap * gm, GameObject * obj,
                            double ang, float x, float y, float tx, float ty, float sp, float de,
                            float dmg, GameObject * sh, bool f)
        : sgl::graphic::TexturedParticleData(x, y, sp, de, t)
        , map(gm)
        , target(obj)
        , angle(ang)
        , targetX(tx)
        , targetY(ty)
        , damage(dmg)
        , shooter(sh)
        , fatal(f)
    {
    }

    const GameMap * map = nullptr;
    GameObject * target = nullptr;
    GameObject * shooter;
    double angle = 0;
    float targetX = 0.f;
    float targetY = 0.f;
    float damage = 0.f;
    bool fatal = false;
};

} // namespace game
