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

	void Clear();
	void Finalize();

private:
	void * mRenderer = nullptr;
};

} // namespace graphic
} // namespace lib
