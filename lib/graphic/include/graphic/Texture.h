#pragma once

struct SDL_Texture;

namespace lib
{
namespace graphic
{

class Image;

enum TextureQuality : int;

class Texture
{
public:
    Texture(const char * file, TextureQuality q);
    ~Texture();

    int GetWidth() const;
    int GetHeight() const;

private:
    void SetTextureQuality(TextureQuality q);

private:
    SDL_Texture * mData = nullptr;

    int mW = 0;
    int mH = 0;

    // access SDL Texture data
    friend class Image;
};

inline int Texture::GetWidth() const { return mW; }
inline int Texture::GetHeight() const { return mH; }

} // namespace graphic
} // namespace lib
