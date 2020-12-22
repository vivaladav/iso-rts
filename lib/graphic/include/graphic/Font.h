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
    bool IsValid() const;

    int GetSize() const;
    int GetStyle() const;

private:
    Font(const char * file, int size, int style);
    ~Font();

    TTF_Font * GetSysFont() const;

private:
    TTF_Font * mSysFont = nullptr;

    int mSize = 0;

    // Only FontManager can create/destroy a Font
    friend class FontManager;
    // Text needs to access system data
    friend class Text;
};

inline bool Font::IsValid() const { return mSysFont != nullptr; }

inline int Font::GetSize() const { return mSize; }

inline TTF_Font * Font::GetSysFont() const { return mSysFont; }

} // namespace graphic
} // namespace lib

