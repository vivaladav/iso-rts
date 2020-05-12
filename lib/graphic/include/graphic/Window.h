namespace lib
{
namespace graphic
{

class Renderer;

class Window
{
public:
    Window(const char * title, int w, int h);
    ~Window();

private:
    void * mWin = nullptr;

    // Renderer needs to access the low level window
    friend class Renderer;
};

} // namespace graphic
} // namespace lib
