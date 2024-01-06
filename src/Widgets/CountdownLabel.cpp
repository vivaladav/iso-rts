#include "Widgets/CountdownLabel.h"

#include <sgl/graphic/Camera.h>
#include <sgl/graphic/Font.h>
#include <sgl/graphic/FontManager.h>
#include <sgl/graphic/Text.h>

#include <sstream>

#include <cmath>

namespace game
{

CountdownLabel::CountdownLabel(PlayerFaction faction, int seconds, sgl::sgui::Widget * parent)
    : sgl::sgui::Widget(parent)
    , mFaction(faction)
    , mTime(seconds)
    , mTimeInt(seconds)
{
    SetCamera(sgl::graphic::Camera::GetDefaultCamera());

    UpdateDigits();
}

void CountdownLabel::UpdateDigits()
{
    using namespace sgl;

    UnregisterRenderable(mTxt);
    delete mTxt;

    // PREPARE DATA
    const int secsInM = 60;
    // mins
    int time = mTimeInt;
    const int timeM = time / secsInM;
    // secs
    time -= timeM * secsInM;
    const int timeS = time;

    const int fieldW = 2;
    const char fieldF = '0';
    std::ostringstream ss;
    ss.width(fieldW);
    ss.fill(fieldF);
    ss << timeM << ":";
    ss.width(fieldW);
    ss.fill(fieldF);
    ss << timeS;

    // CREATE TEXT
    const int fontSize = 20;
    const unsigned int colors[] = { 0xd98c8cff, 0x8cd98cff, 0x8cccd9ff };

    auto fm = graphic::FontManager::Instance();
    graphic::Font * font = fm->GetFont("Lato-Bold.ttf", fontSize, graphic::Font::NORMAL);

    mTxt = new graphic::Text(ss.str().c_str(), font);
    mTxt->SetColor(colors[mFaction]);
    RegisterRenderable(mTxt);

    // update widget size
    SetSize(mTxt->GetWidth(), mTxt->GetHeight());

    // re-position text
    SetPositions();
}

void CountdownLabel::HandlePositionChanged()
{
    SetPositions();
}

void CountdownLabel::SetPositions()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    mTxt->SetPosition(x0, y0);
}

void CountdownLabel::OnUpdate(float delta)
{
    mTime -= delta;

    if(mTime < 0.f)
        mTime = 0.f;

    const int t = static_cast<int>(std::roundf(mTime));

    if(t != mTimeInt)
    {
        mTimeInt = t;
        UpdateDigits();
    }
}

} // namespace game
