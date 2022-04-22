#include "xEasyGL.h"

using namespace xlonlat;
using namespace xlonlat::xEasyGL;

class ViewerPro : public xViewer
{
public:
	virtual ~ViewerPro() {}

	virtual void Render(double interval) override
    {
		float width = (float)(m_drawArgs->vs().w);
		float height = (float)(m_drawArgs->vs().h);

		glClearDepth(1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		Begin2D(*m_drawArgs);
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

	virtual void OnLButtonDown(int cx, int cy) override
	{
		std::cout << "OnLButtonDown:" << cx << "," << cy << std::endl;
	}

	virtual void OnRButtonDown(int cx, int cy) override
	{
		std::cout << "OnRButtonDown:" << cx << "," << cy << std::endl;
	}

	virtual void OnLButtonUp(int cx, int cy) override
	{
		std::cout << "OnLButtonUp:" << cx << "," << cy << std::endl;
	}

	virtual void OnRButtonUp(int cx, int cy) override
	{
		std::cout << "OnRButtonUp:" << cx << "," << cy << std::endl;
	}

	virtual void OnMouseMove(int cx, int cy, int button) override
	{
		std::cout << "OnMouseMove:" << cx << "," << cy << ",button=" << button << std::endl;
	}

	virtual void OnMouseWheel(int cx, int cy, bool zoomin) override
	{
		std::cout << "OnMouseWheel:" << cx << "," << cy << ",zoomin=" << zoomin << std::endl;
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
    xWindow window(new ViewerPro);
    window.Run();

    _CrtDumpMemoryLeaks();
    return 0;
}