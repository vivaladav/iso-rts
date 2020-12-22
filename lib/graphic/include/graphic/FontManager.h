#pragma once

#include <string>
#include <unordered_map>

namespace lib
{
namespace graphic
{

class Font;

class FontManager
{
public:
    static FontManager * Create();
    static FontManager * Instance();
    static void Destroy();

    Font * GetFont(const char * file, int size, int style);

    void ClearFonts();

private:
    FontManager();
    ~FontManager();

private:
    static FontManager * mInstance;

    std::unordered_map<std::string, Font *> mFonts;
};

inline FontManager * FontManager::Instance() { return mInstance; }

inline FontManager::FontManager() { }
inline FontManager::~FontManager() { ClearFonts(); }

} // namespace graphic
} // namespace lib
