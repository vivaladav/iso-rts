#pragma once

#include "Collectable.h"

namespace game
{

class BlobsGenerator;

class Blobs : public Collectable
{
public:
    Blobs();

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

inline Blobs::Blobs() : Collectable(GameObject::TYPE_BLOBS, 1, 1)
{
    SetObjColors();
}

inline int Blobs::GetNum() const { return mNum; }

} // namespace game
