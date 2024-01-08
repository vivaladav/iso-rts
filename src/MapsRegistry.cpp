#include "MapsRegistry.h"

#include "GameConstants.h"

namespace game
{

constexpr int MAX_MAPS_PER_PLANET = 12;

MapsRegistry::MapData::MapData(const std::string & file, int energy, int material, int diamonds,
                               int blobs, int size, int value, PlayerFaction occupier,
                               TerritoryStatus status, MissionType mission)
    : mFile(file)
    , mEnergy(energy)
    , mMaterial(material)
    , mDiamonds(diamonds)
    , mBlobs(blobs)
    , mSize(size)
    , mValue(value)
    , mOccupier(occupier)
    , mStatus(status)
    , mMission(mission)
{
}

bool MapsRegistry::CreatePlanet(unsigned int planetId)
{
    // planet already added
    if(mData.find(planetId) != mData.end())
        return false;

     mData.emplace(planetId, std::vector<MapData>());
     mData[planetId].reserve(MAX_MAPS_PER_PLANET);

     return true;
}

bool MapsRegistry::AddMap(unsigned int planetId, const std::string & file, int energy,
                          int material, int diamonds, int blobs, int size, int value,
                          PlayerFaction occupier, TerritoryStatus status, MissionType mission)
{
    // planet not found
    if(mData.find(planetId) == mData.end())
        return false;

    mData[planetId].emplace_back(file, energy, material, diamonds,
                                 blobs, size, value, occupier, status, mission);

    return true;
}

bool MapsRegistry::AddUnavailableMap(unsigned int planetId)
{
    // planet not found
    if(mData.find(planetId) == mData.end())
        return false;

    mData[planetId].emplace_back(std::string(), 0, 0, 0, 0, 0, 0,
                                 NO_FACTION, TER_ST_UNAVAILABLE, MISSION_UNKNOWN);

    return true;
}

int MapsRegistry::GetNumMaps(unsigned int planetId) const
{
    // planet not found
    if(mData.find(planetId) == mData.end())
        return 0;

    return mData.at(planetId).size();
}

const std::string & MapsRegistry::GetMapFile(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mFile;
    }

    static const std::string empty;
    return empty;
}

int MapsRegistry::GetMapEnergy(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mEnergy;
    }

    return 0;
}

int MapsRegistry::GetMapMaterial(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mMaterial;
    }

    return 0;
}

int MapsRegistry::GetMapDiamonds(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mDiamonds;
    }

    return 0;
}

int MapsRegistry::GetMapBlobs(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mBlobs;
    }
    return 0;
}

int MapsRegistry::GetMapSize(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mSize;
    }

    return 0;
}

int MapsRegistry::GetMapValue(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mValue;
    }

    return 0;
}

PlayerFaction MapsRegistry::GetMapOccupier(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mOccupier;
    }

    return NO_FACTION;
}

TerritoryStatus MapsRegistry::GetMapStatus(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mStatus;
    }

    return TER_ST_UNKNOWN;
}

MissionType MapsRegistry::GetMapMission(unsigned int planetId, unsigned int index) const
{
    if(mData.find(planetId) != mData.end())
    {
        const auto & data = mData.at(planetId);

        if(index < data.size())
            return data[index].mMission;
    }

    return MISSION_UNKNOWN;
}

void MapsRegistry::SetMapStatus(unsigned int planetId, unsigned int index, TerritoryStatus status)
{
    if(mData.find(planetId) != mData.end())
    {
        auto & data = mData.at(planetId);

        if(index < data.size())
            data[index].mStatus = status;
    }
}

void MapsRegistry::SetMapOccupier(unsigned int planetId, unsigned int index, PlayerFaction occupier)
{
    if(mData.find(planetId) != mData.end())
    {
        auto & data = mData.at(planetId);

        if(index < data.size())
            data[index].mOccupier = occupier;
    }
}

void MapsRegistry::SetMapMissionCompleted(unsigned int planetId, unsigned int index)
{
    if(mData.find(planetId) != mData.end())
    {
        auto & data = mData.at(planetId);

        if(index < data.size())
            data[index].mMission = MISSION_COMPLETED;
    }
}

void MapsRegistry::ClearData()
{
    mData.clear();
}

} // namespace game
