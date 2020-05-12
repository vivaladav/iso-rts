namespace lib
{
namespace graphic
{

class Window;

class Renderer
{
public:
    Renderer(Window * win);
    ~Renderer();

    void SetRenderColor(unsigned char r, unsigned char g,
                        unsigned char b, unsigned char a);

    void Clear();

    void Finalize();

private:
    void * mRenderer = nullptr;
};

} // namespace graphic
} // namespace lib
