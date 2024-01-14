#pragma once

#include "Structure.h"

namespace game
{

class Temple : public Structure
{
public:
    Temple();

    void SetInvestedResources(int money, int material, int blobs, int diamonds);
    int GetExplorationTime() const;
    int GetExplorationSuccessRate() const;

private:
    void UpdateGraphics() override;

    void SetImage();
    void SetObjColors();

private:
    int mExplorationTime = 0;
    int mExplorationSuccess = 0;
};

inline int Temple::GetExplorationTime() const { return mExplorationTime; }
inline int Temple::GetExplorationSuccessRate() const { return mExplorationSuccess; }

} // namespace game
