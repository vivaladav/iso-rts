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
    void UpdateImage() override;

private:
    void SetImage();

private:
    int mNum = 0;
};

inline Blobs::Blobs() : Collectable(GameObjectType::OBJ_BLOBS, 1, 1)
{
}

inline int Blobs::GetNum() const { return mNum; }

} // namespace game
