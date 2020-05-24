#pragma once

struct SDL_Window;

namespace lib
{
namespace graphic
{

class Renderer;

class Window
{
public:
    static Window * Create(const char * title, int w, int h);
    static Window * Instance();
    static void Destroy();

    int GetWidth() const;
    int GetHeight() const;
    void SetSize(int w, int h);

    bool IsFullscreen() const;
    void SetFullscreen(bool f);
    void SwitchFullscreen();

private:
    Window(const char * title, int w, int h);
    ~Window();

private:
    static Window * mInstance;

    int mW = 0;
    int mH = 0;

    SDL_Window * mSysWin = nullptr;

    bool mFullscreen = false;

    // Renderer needs to access the low level window
    friend class Renderer;
};

inline Window * Window::Instance() { return mInstance; }

inline bool Window::IsFullscreen() const { return mFullscreen; }
inline void Window::SwitchFullscreen()
{
    SetFullscreen(!mFullscreen);
}

inline int Window::GetWidth() const { return mW; }
inline int Window::GetHeight() const { return mH; }

} // namespace graphic
} // namespace lib
