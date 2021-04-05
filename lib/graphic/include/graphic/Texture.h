#pragma once

#include <memory>

struct SDL_Rect;
struct SDL_Surface;
struct SDL_Texture;

namespace lib
{
namespace graphic
{

class TextureData;

enum TextureQuality : int;

class Texture
{
public:
    Texture(SDL_Surface * data, TextureQuality q);
    Texture(const char * file, TextureQuality q);
    Texture(const std::shared_ptr<TextureData> & data);
    ~Texture();

    int GetWidth() const;
    int GetHeight() const;

    void SetSourceRect(int x, int y, int w, int h);

    void SetAlpha(unsigned char a);
    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

private:
    void InitSrcRect();

private:
    std::shared_ptr<TextureData> mTexData;
    SDL_Rect * mSrcRect = nullptr;

    // access private data
    friend class TexturedRenderable;
};

} // namespace graphic
} // namespace lib
