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

    void MoveX(float delta);
    void MoveY(float delta);

    void ResetPosition();

    // conversion
    int GetScreenToWorldX(int x) const;
    int GetScreenToWorldY(int y) const;

    // default camera
    static void CreateDefaultCamera();
    static Camera * GetDefaultCamera();
    static void DestroyDefaultCamera();

    // dummy camera
    static void CreateDummyCamera();
    static Camera * GetDummyCamera();
    static void DestroyDummyCamera();

private:
    static Camera * mDummy;
    static Camera * mDefault;

    float mXf = 0.f;
    float mYf = 0.f;
    int mXd = 0;
    int mYd = 0;
    int mWidth = 0;
    int mHeight = 0;
};

inline Camera::Camera(int w, int h)
    : mWidth(w)
    , mHeight(h)
{
}

inline int Camera::GetX() const { return mXd; }
inline int Camera::GetY() const { return mYd; }
inline int Camera::GetWidth() const { return mWidth; }
inline int Camera::GetHeight() const { return mHeight; }

inline void Camera::SetWidth(int w) { mWidth = w; }
inline void Camera::SetHeight(int h) { mHeight = h; }

inline void Camera::ResetPosition()
{
    mXf = 0.f;
    mYf = 0.f;

    mXd = 0;
    mYd = 0;
}

inline int Camera::GetScreenToWorldX(int x) const { return x + mXd; }
inline int Camera::GetScreenToWorldY(int y) const { return y + mYd; }

inline Camera * Camera::GetDefaultCamera() { return mDefault; }

inline Camera * Camera::GetDummyCamera() { return mDummy; }

} // namespace graphic
} // namespace lib
