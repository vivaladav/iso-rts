#pragma once

typedef struct _TTF_Font TTF_Font;
class Text;

namespace lib
{
namespace graphic
{

class Font
{
public:
    Font(const char * file, int size);
    ~Font();

    int GetSize() const;

    void SetBold(bool val);
    void SetItalic(bool val);
    void SetStrikethrough(bool val);
    void SetUnderline(bool val);

private:
    void UpdateStyle();

private:
    TTF_Font * mSysFont = nullptr;

    int mSize = 0;

    bool mBold = false;
    bool mItalic = false;
    bool mStrikethrough = false;
    bool mUnderline = false;
};

inline int Font::GetSize() const { return mSize; }

} // namespace graphic
} // namespace lib

