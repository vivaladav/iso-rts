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

Font * FontManager::GetFont(const char * file, int size, int style)
{
    // search if fonts already exists
    const std::string key = std::string(file) +
                            "sz" + std::to_string(size) +
                            "st" + std::to_string(style);

    auto res = mFonts.find(key);

    if(res != mFonts.end())
        return res->second;

    // font not found -> create it
    Font * font = new Font(file, size, style);

    if(!font->IsValid())
    {
        delete font;
        return nullptr;
    }

    mFonts.emplace(key, font);

    return font;
}

void FontManager::ClearFonts()
{
    for(auto & it : mFonts)
        delete it.second;

    mFonts.clear();
}

} // namespace graphic
} // namespace lib
