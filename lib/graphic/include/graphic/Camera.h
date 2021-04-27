#pragma once

namespace lib
{
namespace graphic
{

class Camera
{
public:
    Camera() = default;
    Camera(int w, int h);

    // camera data
    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;

    void SetWidth(int w);
    void SetHeight(int h);

    void MoveX(int delta);
    void MoveY(int delta);

    // conversion
    int GetScreenToWorldX(int x) const;
    int GetScreenToWorldY(int y) const;

    // dummy camera
    static void CreateDummyCamera();
    static Camera * GetDummyCamera();
    static void DestroyDummyCamera();

private:
    static Camera * mDummy;

    int mX = 0;
    int mY = 0;
    int mWidth = 0;
    int mHeight = 0;
};

inline Camera::Camera(int w, int h)
    : mWidth(w)
    , mHeight(h)
{
}

inline int Camera::GetX() const { return mX; }
inline int Camera::GetY() const { return mY; }
inline int Camera::GetWidth() const { return mWidth; }
inline int Camera::GetHeight() const { return mHeight; }

inline void Camera::SetWidth(int w) { mWidth = w; }
inline void Camera::SetHeight(int h) { mHeight = h; }

inline void Camera::MoveX(int delta) { mX += delta; }
inline void Camera::MoveY(int delta) { mY += delta; }

inline int Camera::GetScreenToWorldX(int x) const { return x + mX; }
inline int Camera::GetScreenToWorldY(int y) const { return y + mY; }

inline Camera * Camera::GetDummyCamera() { return mDummy; }

} // namespace graphic
} // namespace lib
