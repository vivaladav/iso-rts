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
    void SetNum(int num);

public:
    static const int MIN_UNITS = 1;
    static const int MAX_UNITS = 4;

protected:
    void UpdateGraphics() override;

private:
    void SetImage();

    void SetObjColors();

private:
    int mNum = 0;
};

inline Diamonds::Diamonds() : Collectable(GameObjectType::OBJ_DIAMONDS, 1, 1)
{
    SetObjColors();
}

inline int Diamonds::GetNum() const { return mNum; }

} // namespace game
