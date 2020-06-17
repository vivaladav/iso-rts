#pragma once

typedef struct _TTF_Font TTF_Font;

namespace lib
{
namespace graphic
{

class Font
{
public:
    static const int NORMAL;
    static const int BOLD;
    static const int ITALIC;
    static const int STRIKETHROUGH;
    static const int UNDERLINE;

public:
    Font(const char * file, int size);
    ~Font();

    int GetSize() const;

    void SetStyle(int s);

private:
    TTF_Font * mSysFont = nullptr;

    int mSize = 0;

    int mStyle;

    // Text needs to access system data
    friend class Text;
};

inline int Font::GetSize() const { return mSize; }

} // namespace graphic
} // namespace lib

