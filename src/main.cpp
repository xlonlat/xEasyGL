#include "xEasyGL.h"

using namespace xlonlat;
using namespace xlonlat::xEasyGL;

class ViewerPro : public xViewer
{
public:
	virtual ~ViewerPro() {}

	virtual void Render() override
    {
		float width = (float)(m_drawArgs->vs().w);
		float height = (float)(m_drawArgs->vs().h);

		glClearDepth(1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		Begin2D();
		{
			glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(width, height, 0.0f);
			glEnd();
		}

#ifdef _DEBUG
		//std::cout << "interval:" << interval << std::endl;
#endif // _DEBUG
    }

	virtual void OnLButtonDown(const xMousePos& pos) override
	{
		std::cout << "OnLButtonDown:" << pos.x << "," << pos.y << std::endl;
	}

	virtual void OnRButtonDown(const xMousePos& pos) override
	{
		std::cout << "OnRButtonDown:" << pos.x << "," << pos.y << std::endl;
	}

	virtual void OnLButtonUp(const xMousePos& pos) override
	{
		std::cout << "OnLButtonUp:" << pos.x << "," << pos.y << std::endl;
	}

	virtual void OnRButtonUp(const xMousePos& pos) override
	{
		std::cout << "OnRButtonUp:" << pos.x << "," << pos.y << std::endl;
	}

	virtual void OnMouseMove(const xMousePos& pos, int button) override
	{
		std::cout << "OnMouseMove:" << pos.x << "," << pos.y << ",button=" << button << std::endl;
	}

	virtual void OnMouseWheel(const xMousePos& pos, bool zoomin) override
	{
		std::cout << "OnMouseWheel:" << pos.x << "," << pos.y << ",zoomin=" << zoomin << std::endl;
	}

	virtual void OnKeyDown(int key) override
	{
		std::cout << "OnKeyDown:" << key << std::endl;
	}

	virtual void OnKeyUp(int key) override
	{
		std::cout << "OnKeyUp:" << key << std::endl;
	}
};

int main()
{
	std::wcout.imbue(std::locale("chs"));

	{
		xWindow window(new xViewer);
		window.Run();
	}

    _CrtDumpMemoryLeaks();
    return 0;
}