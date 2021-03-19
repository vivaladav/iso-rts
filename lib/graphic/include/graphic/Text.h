#pragma once

#include "graphic/TexturedRenderable.h"

namespace lib
{
namespace graphic
{

class Font;

class Text : public TexturedRenderable
{
public:
    Text(const char * text, Font * font, bool trim = false);
    ~Text();

    int GetPaddingTop() const;
    int GetPaddingBottom() const;

private:
    int mPaddingTop = 0;
    int mPaddingBottom = 0;
};

inline int Text::GetPaddingTop() const { return mPaddingTop; }
inline int Text::GetPaddingBottom() const { return mPaddingBottom; }

} // namespace graphic
} // namespace lib

