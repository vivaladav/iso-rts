#include "MapLoader.h"

#include "GameConstants.h"

#include <fstream>
#include <sstream>

namespace game
{

const std::string MapLoader::MAP_VERSION("0.1.1");

void MapLoader::Clear()
{
    mObjEntries.clear();
    mCellTypes.clear();

    mVer.clear();

    mRows = 0 ;
    mCols = 0 ;
}

bool MapLoader::Load(const std::string & filename)
{
    // open map file
    std::fstream fs(filename);

    if(!fs.is_open())
        return false;

    ReadBaseData(fs);

    ReadObjectsData(fs);

    fs.close();

    return true;
}

void MapLoader::ReadBaseData(std::fstream & fs)
{
    std::string line;
    std::istringstream ss;

    // reading file version
    std::getline(fs, line);
    ss.str(line);

    ss >> mVer;
    ss.clear();

    // reading mission data
    std::getline(fs, line);
    ss.str(line);

    unsigned int mission;
    ss >> mission;
    mMissionType = static_cast<MissionType>(mission);

    if(MISSION_RESIST_TIME == mMissionType)
        ss >> mMissionTime;

    ss.clear();

    // reading map size
    std::getline(fs, line);
    ss.str(line);

    ss >> mRows >> mCols;

    // READ BASE MAP
    for(unsigned int r = 0; r < mRows; ++r)
    {
        std::getline(fs, line);
        ss.clear();
        ss.str(line);

        for(unsigned int c = 0; c < mCols; ++c)
        {
            unsigned int type;
            ss >> type;

            mCellTypes.push_back(type);
        }
    }
}

void MapLoader::ReadObjectsData(std::fstream & fs)
{
    std::string line;
    std::istringstream ss;

    // READ OBJECTS
    while(std::getline(fs, line))
    {
        // skip comments
        if(!line.empty() && '#' == line[0])
            continue;

        ss.clear();
        ss.str(line);

        MapObjectEntry e;
        std::string objIdStr;

        ss >> e.layerId >> objIdStr >> e.variantId >> e.faction >> e.r0 >> e.c0;
        e.typeId = std::hash<std::string>{}(objIdStr);

        mObjEntries.emplace_back(e);
    }
}

} // namespace game
