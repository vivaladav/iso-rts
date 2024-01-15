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

    int GetMaxInvestableMoney() const;
    int GetMaxInvestableMaterial() const;
    int GetMaxInvestableBlobs() const;
    int GetMaxInvestableDiamonds() const;

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

inline int Temple::GetMaxInvestableMoney() const { return 9999; }
inline int Temple::GetMaxInvestableMaterial() const { return 5000; }
inline int Temple::GetMaxInvestableBlobs() const { return 400; }
inline int Temple::GetMaxInvestableDiamonds() const { return 400; }

} // namespace game
