#pragma once

#include <string>
#include <unordered_map>

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

    void SetStyle(int s);

private:
    Font(const char * file, int size);
    ~Font();

    void CreateSysFont();

    TTF_Font * GetSyFont() const;

private:
    std::string mFile;
    std::unordered_map<int, TTF_Font *> mSysFonts;
    TTF_Font * mActiveSysFont = nullptr;

    int mSize = 0;

    int mStyle;

    // Only FontManager can create/destroy a Font
    friend class FontManager;
    // Text needs to access system data
    friend class Text;
};

inline bool Font::IsValid() const { return mActiveSysFont != nullptr; }

inline int Font::GetSize() const { return mSize; }

inline TTF_Font * Font::GetSyFont() const { return mActiveSysFont; }

} // namespace graphic
} // namespace lib

