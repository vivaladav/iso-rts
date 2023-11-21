#pragma once

#include <string>

namespace game
{

class GameMap;
class IsoMap;

class MapLoader
{
public:
    static const std::string MAP_VERSION;

public:
    MapLoader(GameMap * gm, IsoMap * im);

    void SetMaps(GameMap * gm, IsoMap * im);

    bool Load(const std::string & filename);

private:
    void ReadBaseData(std::fstream & fs);
    void ReadObjectsData(std::fstream & fs);

private:
    GameMap * mGameMap = nullptr;
    IsoMap * mIsoMap = nullptr;
};

inline MapLoader::MapLoader(GameMap * gm, IsoMap * im)
    : mGameMap(gm)
    , mIsoMap(im)
{
}

inline void MapLoader::SetMaps(GameMap * gm, IsoMap * im)
{
    mGameMap = gm;
    mIsoMap = im;
}

} // namespace game
