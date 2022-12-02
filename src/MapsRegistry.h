#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace game
{

enum PlayerFaction : unsigned int;
enum TerritoryStatus : unsigned int;

class MapsRegistry
{
public:
    bool CreatePlanet(unsigned int planetId);

    bool AddMap(unsigned int planetId, const std::string & file, int energy,
                int material, int diamonds, int blobs, int size, int value,
                PlayerFaction occupier, TerritoryStatus status);
    bool AddUnavailableMap(unsigned int planetId);

    int GetNumMaps(unsigned int planetId) const;

    const std::string & GetMapFile(unsigned int planetId, unsigned int index) const;
    int GetMapEnergy(unsigned int planetId, unsigned int index) const;
    int GetMapMaterial(unsigned int planetId, unsigned int index) const;
    int GetMapDiamonds(unsigned int planetId, unsigned int index) const;
    int GetMapBlobs(unsigned int planetId, unsigned int index) const;
    int GetMapSize(unsigned int planetId, unsigned int index) const;
    int GetMapValue(unsigned int planetId, unsigned int index) const;
    PlayerFaction GetMapOccupier(unsigned int planetId, unsigned int index) const;
    TerritoryStatus GetMapStatus(unsigned int planetId, unsigned int index) const;

    void SetMapStatus(unsigned int planetId, unsigned int index, TerritoryStatus status);
    void SetMapOccupier(unsigned int planetId, unsigned int index, PlayerFaction occupier);

private:
    struct MapData
    {
        MapData(const std::string & file, int energy,
                int material, int diamonds, int blobs, int size, int value,
                PlayerFaction occupier, TerritoryStatus status);

        std::string mFile;
        int mEnergy;
        int mMaterial;
        int mDiamonds;
        int mBlobs;
        int mSize;
        int mValue;
        PlayerFaction mOccupier;
        TerritoryStatus mStatus;
    };

    std::unordered_map<unsigned int, std::vector<MapData>> mData;
};

} // namespace game
