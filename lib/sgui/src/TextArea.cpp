#include "sgui/TextArea.h"

#include "graphic/DummyRenderable.h"
#include "graphic/Font.h"
#include "graphic/Text.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cassert>

namespace lib
{
namespace sgui
{

TextArea::TextArea(int w, int h, graphic::Font * font, Widget * parent)
    : Widget(parent)
    , mFont(font)
{
    assert(font);

    SetSize(w, h);
}

TextArea::TextArea(int w, int h, const char * txt, graphic::Font * font, Widget * parent)
    : Widget(parent)
    , mStr(txt)
    , mFont(font)
{
    assert(txt);
    assert(font);

    SetSize(w, h);

    CreateText();
}

void TextArea::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    a = MixAlphaAndAlpha(a);

    mTxtR = r;
    mTxtG = g;
    mTxtB = b;
    mTxtA = a;

    for(lib::graphic::Renderable * txt : mTxtLines)
        txt->SetColor(r, g, b, a);
}

void TextArea::SetColor(unsigned int color)
{
    color = MixColorAndAlpha(color);

    mTxtR = (color & 0xFF000000) >> 24;
    mTxtG = (color & 0x00FF0000) >> 16;
    mTxtB = (color & 0x0000FF00) >> 8;
    mTxtA = color & 0x000000FF;

    for(lib::graphic::Renderable * txt : mTxtLines)
        txt->SetColor(color);
}

void TextArea::ClearText()
{
    for(lib::graphic::Renderable * txt : mTxtLines)
    {
        UnregisterRenderable(txt);
        delete txt;
    }

    mTxtLines.clear();
}

void TextArea::SetText(const char * txt)
{
    assert(txt);

    std::string t(txt);

    // setting same text -> exit
    if(t == mStr)
        return ;

    ClearText();

    mStr = t;

    CreateText();

    HandlePositionChanged();

    SetColor(mTxtR, mTxtG, mTxtB, mTxtA);
}

void TextArea::CreateText()
{
    const int len = mStr.length();
    int ind0 = 0;
    int ind = 0;
    int lastIndOk = 0;
    int h = 0;

    while(ind0 < len)
    {
        bool newline = false;

        // find next break
        while(ind < len)
        {
            if(' ' == mStr[ind])
                break;
            else if('\n' == mStr[ind])
            {
                newline = true;
                break;
            }

            ++ind;
        }

        // check current substring
        std::string sub = mStr.substr(ind0, ind - ind0);

        int lineW = 0;
        int lineH = 0;
        TTF_SizeUTF8(mFont->GetSysFont(), sub.c_str(), &lineW, &lineH);

        // exit if passed allowed H
        if(h + lineH > GetHeight())
            return ;

        // line shorter than widget
        if(lineW < GetWidth())
        {
            // add line because of \n OR finished
            if(newline || ind == len)
            {
                // empty line in case of \n\n
                if(sub.empty())
                    AddEmptyLine(lineH);
                else
                    AddTextLine(sub.c_str());

                ind0 = ++ind;
                lastIndOk = ind0;

                h += lineH;
            }
            // update limit
            else
            {
                lastIndOk = ind;
                ++ind;
            }
        }
        // line longer than widget -> render to previous ok limit
        else
        {
            std::string sub = mStr.substr(ind0, lastIndOk - ind0);

            AddTextLine(sub.c_str());

            ind0 = ++lastIndOk;
            ind = ind0;

            h += lineH;
        }
    }
}

void TextArea::AddTextLine(const char * txt)
{
    auto line = new lib::graphic::Text(txt, mFont, false);
    RegisterRenderable(line);
    mTxtLines.push_back(line);
}

void TextArea::AddEmptyLine(int h)
{
    auto line = new lib::graphic::DummyRenderable;
    line->SetHeight(h);
    RegisterRenderable(line);
    mTxtLines.push_back(line);
}

void TextArea::HandlePositionChanged()
{
    const int x0 = GetScreenX();
    const int y0 = GetScreenY();

    int x = x0;
    int y = y0;

    // set starting Y based on vertical alignment
    if(ALIGN_V_CENTER == mAlignV || ALIGN_V_BOTTOM == mAlignV)
    {
        int totH = 0;

        for(lib::graphic::Renderable * txt : mTxtLines)
            totH += txt->GetHeight();

        if(ALIGN_V_CENTER == mAlignV)
            y = y0 + (GetHeight() - totH) * 0.5f;
        // ALIGN_V_BOTTOM
        else
            y = y0 + GetHeight() - totH;
    }

    // set positions based on horizontal alignment
    if(ALIGN_H_LEFT == mAlignH)
    {
        for(lib::graphic::Renderable * txt : mTxtLines)
        {
            txt->SetPosition(x, y);

            y += txt->GetHeight();
        }
    }
    else if(ALIGN_H_CENTER == mAlignH)
    {
        for(lib::graphic::Renderable * txt : mTxtLines)
        {
            x = x0 + (GetWidth() - txt->GetWidth()) * 0.5f;

            txt->SetPosition(x, y);

            y += txt->GetHeight();
        }
    }
    // ALIGN_H_RIGHT
    else
    {
        for(lib::graphic::Renderable * txt : mTxtLines)
        {
            x = x0 + GetWidth() - txt->GetWidth();

            txt->SetPosition(x, y);

            y += txt->GetHeight();
        }
    }
}

} // namespace sgui
} // namespace lib
