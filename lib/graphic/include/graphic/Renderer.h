#pragma once

struct SDL_Renderer;

namespace lib
{
namespace graphic
{

class Window;

class Renderer
{
public:
    static Renderer * Create(Window * win);
    static Renderer * Instance();
    static void Destroy();

    int GetWidth() const;
    int GetHeight() const;
    bool SetLogicalSize(int w, int h);

    void SetRenderColor(unsigned char r, unsigned char g,
                        unsigned char b, unsigned char a);

    void Clear();
    void Clear(unsigned char r, unsigned char g,
               unsigned char b, unsigned char a);

    void Finalize();

    void RenderTexture();

private:
    Renderer(Window * win);
    ~Renderer();

private:
    static Renderer * mInstance;

    SDL_Renderer * mSysRenderer = nullptr;

    int mW = 0;
    int mH = 0;

    // classess that need to access the low level system renderer
    friend class TexturedRenderable;
    friend class Texture;
};

inline Renderer * Renderer::Instance() { return mInstance; }

inline int Renderer::GetWidth() const { return mW; }
inline int Renderer::GetHeight() const { return mH; }

} // namespace graphic
} // namespace lib
