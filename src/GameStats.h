#pragma once

namespace sgl { namespace services { class Steam; } }

namespace game
{

enum PlanetId : unsigned int;

class GameStats
{
public:
    // NOTE call this only after sgl::services::Steam::Create()
    GameStats();
    // NOTE call this only before sgl::services::Steam::Destroy()
    ~GameStats();

    // update stats
    void IncMissionPlayed(PlanetId planet, int map);
    void IncLoadsPlayed();

    void SetTutorialCompleted();

    // finalize stats
    void PushStats();

private:
    sgl::services::Steam * mSteam = nullptr;

    int mStatVersion = 0;

    int mStatMission0101 = 0;
    int mStatMission0102 = 0;
    int mStatMission0103 = 0;
    int mStatMission01e1 = 0;
    int mStatMission01e2 = 0;

    int mStatMissionsPlayed = 0;
    int mStatLoadsPlayed = 0;

    int mTutorialCompleted = 0;
};

} // namespace game