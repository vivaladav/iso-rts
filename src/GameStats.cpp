#include "GameStats.h"

#include "Planet.h"

#include <sgl/services/Steam.h>

#include <iostream>

namespace
{
const char * keyVersion = "VERSION";

const char * keyMission0101 = "mission_01-01";
const char * keyMission0102 = "mission_01-02";
const char * keyMission0103 = "mission_01-03";
const char * keyMission01e1 = "mission_01-e1";
const char * keyMission01e2 = "mission_01-e2";
const char * keyMissionsPlayed = "missions_played";
const char * keyLoadsPlayed = "loads_played";
const char * keyTutorialCompleted = "tut_completed";
}

namespace game
{

GameStats::GameStats()
    : mSteam(sgl::services::Steam::Instance())
{
    // -- VERSION --
    if(mSteam->GetStat(keyVersion, &mStatVersion))
        std::cout << "GameStats - " << keyVersion << ": " << mStatVersion << std::endl;
    else
        std::cout << "[ERR] GameStats - " << keyVersion << " GET FAILED" << std::endl;

    // -- MISSIONS --
    if(!mSteam->GetStat(keyMission0101, &mStatMission0101))
        std::cout << "[ERR] GameStats - " << keyMission0101 << " GET FAILED" << std::endl;

    if(!mSteam->GetStat(keyMission0102, &mStatMission0102))
        std::cout << "[ERR] GameStats - " << keyMission0102 << " GET FAILED" << std::endl;

    if(!mSteam->GetStat(keyMission0103, &mStatMission0103))
        std::cout << "[ERR] GameStats - " << keyMission0103 << " GET FAILED" << std::endl;

    if(!mSteam->GetStat(keyMission01e1, &mStatMission01e1))
        std::cout << "[ERR] GameStats - " << keyMission01e1 << " GET FAILED" << std::endl;

    if(!mSteam->GetStat(keyMission01e2, &mStatMission01e2))
        std::cout << "[ERR] GameStats - " << keyMission01e2 << " GET FAILED" << std::endl;

    if(!mSteam->GetStat(keyMissionsPlayed, &mStatMissionsPlayed))
        std::cout << "[ERR] GameStats - " << keyMissionsPlayed << " GET FAILED" << std::endl;

    if(!mSteam->GetStat(keyLoadsPlayed, &mStatLoadsPlayed))
        std::cout << "[ERR] GameStats - " << keyLoadsPlayed << " GET FAILED" << std::endl;

    // -- TUTORIAL --
    if(!mSteam->GetStat(keyTutorialCompleted, &mTutorialCompleted))
        std::cout << "[ERR] GameStats - " << keyTutorialCompleted << " GET FAILED" << std::endl;
}

GameStats::~GameStats()
{
    PushStats();
}

void GameStats::IncMissionPlayed(PlanetId planet, int map)
{
    if(planet == PLANET_1)
    {
        if(map == 0)
        {
            ++mStatMission0101;

            if(!mSteam->SetStat(keyMission0101, mStatMission0101))
                std::cout << "[ERR] GameStats - " << keyMission0101 << " SET FAILED" << std::endl;
        }
        else if(map == 2)
        {
            ++mStatMission0102;

            if(!mSteam->SetStat(keyMission0102, mStatMission0102))
                std::cout << "[ERR] GameStats - " << keyMission0102 << " SET FAILED" << std::endl;
        }
        else if(map == 4)
        {
            ++mStatMission0103;

            if(!mSteam->SetStat(keyMission0103, mStatMission0103))
                std::cout << "[ERR] GameStats - " << keyMission0103 << " SET FAILED" << std::endl;
        }
        else if(map == 1)
        {
            ++mStatMission01e1;

            if(!mSteam->SetStat(keyMission01e1, mStatMission01e1))
                std::cout << "[ERR] GameStats - " << keyMission01e1 << " SET FAILED" << std::endl;
        }
        else if(map == 3)
        {
            ++mStatMission01e2;

            if(!mSteam->SetStat(keyMission01e2, mStatMission01e2))
                std::cout << "[ERR] GameStats - " << keyMission01e2 << " SET FAILED" << std::endl;
        }
    }

    // increment total number of missions played
    ++mStatMissionsPlayed;

    if(!mSteam->SetStat(keyMissionsPlayed, mStatMissionsPlayed))
                std::cout << "[ERR] GameStats - " << keyMissionsPlayed << " SET FAILED" << std::endl;
}

void GameStats::IncLoadsPlayed()
{
    ++mStatLoadsPlayed;

    if(!mSteam->SetStat(keyLoadsPlayed, mStatLoadsPlayed))
        std::cout << "[ERR] GameStats - " << keyLoadsPlayed << " SET FAILED" << std::endl;
}

void GameStats::SetTutorialCompleted()
{
    if(mTutorialCompleted == 1)
        return ;

    mTutorialCompleted = 1;

    if(!mSteam->SetStat(keyTutorialCompleted, mTutorialCompleted))
        std::cout << "[ERR] GameStats - " << keyTutorialCompleted << " SET FAILED" << std::endl;
}

void GameStats::PushStats()
{
    sgl::services::Steam::Instance()->StoreStatsAndAchievements();
}

} // namespace game