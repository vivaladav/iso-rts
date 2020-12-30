#include "MapLoader.h"

#include "GameMap.h"
#include "IsoMap.h"

#include <sstream>

namespace game
{

bool MapLoader::Load(const std::string & filename)
{
    // map not set
    if(nullptr == mIsoMap)
        return false;

    // open map file
    std::fstream fs(filename);

    if(!fs.is_open())
        return false;

    ReadBaseData(fs);

    ReadObjectsData(fs);

    return true;
}

bool MapLoader::LoadPreview(const std::string & filename)
{
    // map not set
    if(nullptr == mIsoMap)
        return false;

    // open map file
    std::fstream fs(filename);

    if(!fs.is_open())
        return false;

    ReadBaseData(fs);

    return true;
}

void MapLoader::ReadBaseData(std::fstream & fs)
{
    std::string line;
    std::istringstream ss;

    // reading map size
    std::getline(fs, line);
    ss.str(line);

    unsigned int rows = 0;
    unsigned int cols = 0;
    ss >> rows >> cols;

    mIsoMap->SetSize(rows, cols);

    // READ BASE MAP
    for(unsigned int r = 0; r < rows; ++r)
    {
        std::getline(fs, line);
        ss.clear();
        ss.str(line);

        const unsigned int ind0 = r * cols;

        for(unsigned int c = 0; c < cols; ++c)
        {
            unsigned int type;

            ss >> type;

            const unsigned int ind = ind0 + c;

            mIsoMap->SetCellType(ind, type);
        }
    }

    // update game map
    if(mGameMap)
    {
        mGameMap->SetSize(rows, cols);
        mGameMap->SyncWalkableCells();
    }
}

void MapLoader::ReadObjectsData(std::fstream & fs)
{
    if(nullptr == mGameMap)
        return ;

    std::string line;
    std::istringstream ss;

    // READ OBJECTS
    while(std::getline(fs, line))
    {
        ss.clear();
        ss.str(line);

        unsigned int layerId;
        unsigned int objId;
        unsigned int r0;
        unsigned int c0;
        unsigned int rows;
        unsigned int cols;

        ss >> layerId >> objId >> r0 >> c0 >> rows >> cols;

        mGameMap->CreateObject(layerId, objId, r0, c0, rows, cols);
    }
}

} // namespace game
