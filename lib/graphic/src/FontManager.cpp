#include "graphic/FontManager.h"

#include "graphic/Font.h"

namespace lib
{
namespace graphic
{

FontManager * FontManager::mInstance = nullptr;

FontManager * FontManager::Create()
{
    if(!mInstance)
        mInstance = new FontManager;

    return mInstance;
}

void FontManager::Destroy()
{
    delete mInstance;
    mInstance = nullptr;
}

Font * FontManager::GetFont(const char * file, int size)
{
    const std::string key = std::string(file) + std::to_string(size);

    auto res = mFonts.find(key);

    if(res != mFonts.end())
        return res->second;

    Font * font = new Font(file, size);

    if(!font->IsValid())
        return nullptr;

    mFonts.emplace(key, font);

    return font;
}

void FontManager::ClearFonts()
{
    for(auto it : mFonts)
        delete it.second;

    mFonts.clear();
}

} // namespace graphic
} // namespace lib
