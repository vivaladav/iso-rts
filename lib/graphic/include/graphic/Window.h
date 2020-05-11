namespace lib
{
namespace graphic
{

class Window
{
public:
	Window(const char * title, int w, int h);
	~Window();

private:
	void * mWin = nullptr;
};

} // namespace graphic
} // namespace lib
