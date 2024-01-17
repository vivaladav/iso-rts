#pragma once

#include "Collectable.h"

namespace game
{

class DiamondsGenerator;

class Diamonds : public Collectable
{
public:
    Diamonds();

    int GetNum() const;
    void MaximizeUnits();
    void MinimizeUnits();

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void SetObjColors();

private:
    int mNum = 0;
};

inline int Diamonds::GetNum() const { return mNum; }

} // namespace game
