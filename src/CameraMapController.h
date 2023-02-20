#pragma once

#include <sgl/core/Point.h>

namespace sgl
{
    namespace core
    {
        class KeyboardEvent;
        class MouseMotionEvent;
    }

    namespace graphic { class Camera; }
}

namespace game
{

class CameraMapController
{
public:
    CameraMapController(sgl::graphic::Camera * cam);

    const sgl::graphic::Camera * GetCamera() const;

    void SetMapArea(const sgl::core::Pointd2D & t, const sgl::core::Pointd2D & r,
                    const sgl::core::Pointd2D & b, const sgl::core::Pointd2D & l);

    void SetSpeed(float val);

    void CenterCameraToPoint(int x, int y);

    void HandleKeyDown(sgl::core::KeyboardEvent & event);
    void HandleKeyUp(sgl::core::KeyboardEvent & event);
    void HandleMouseMotion(sgl::core::MouseMotionEvent & event);
    void HandleMouseLeftWindow();

    void Update(float delta);

private:
    bool IsPointInsideTL(const sgl::core::Pointd2D & p) const;
    bool IsPointInsideTR(const sgl::core::Pointd2D & p) const;
    bool IsPointInsideBL(const sgl::core::Pointd2D & p) const;
    bool IsPointInsideBR(const sgl::core::Pointd2D & p) const;

    sgl::core::Pointd2D GetVectorPojection(const sgl::core::Pointd2D & a0, const sgl::core::Pointd2D & b0,
                                           const sgl::core::Pointd2D & p) const;

private:
    sgl::graphic::Camera * mCamera = nullptr;

    sgl::core::Pointd2D mMapT;
    sgl::core::Pointd2D mMapR;
    sgl::core::Pointd2D mMapB;
    sgl::core::Pointd2D mMapL;

    float mSpeed;

    int mDirX;
    int mDirY;

    bool mKeyScrollX = false;
    bool mKeyScrollY = false;
    bool mMouseScrollX = false;
    bool mMouseScrollY = false;
};

inline const sgl::graphic::Camera * CameraMapController::GetCamera() const
{
    return mCamera;
}

inline void CameraMapController::SetSpeed(float val) { mSpeed = val; }

inline bool CameraMapController::IsPointInsideTL(const sgl::core::Pointd2D & p) const
{
    return (p.x - mMapT.x) * (mMapL.y - mMapT.y) > (p.y - mMapT.y) * (mMapL.x - mMapT.x);
}

inline bool CameraMapController::IsPointInsideTR(const sgl::core::Pointd2D & p) const
{
    return (p.x - mMapR.x) * (mMapT.y - mMapR.y) > (p.y - mMapR.y) * (mMapT.x - mMapR.x);
}

inline bool CameraMapController::IsPointInsideBL(const sgl::core::Pointd2D & p) const
{
    return (p.x - mMapL.x) * (mMapB.y - mMapL.y) > (p.y - mMapL.y) * (mMapB.x - mMapL.x);
}

inline bool CameraMapController::IsPointInsideBR(const sgl::core::Pointd2D & p) const
{
    return (p.x - mMapB.x) * (mMapR.y - mMapB.y) > (p.y - mMapB.y) * (mMapR.x - mMapB.x);;
}

} // namespace game
