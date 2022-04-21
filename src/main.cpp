#include "xEasyGL.h"

using namespace xlonlat;
using namespace xlonlat::xEasyGL;

class ViewerPro : public xViewer
{
public:
	virtual ~ViewerPro() {}
	virtual void Render() override
    {
		float width = (float)(m_drawArgs->vs().width);
		float height = (float)(m_drawArgs->vs().height);

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
    }
};

int main()
{
    xWindow window(new ViewerPro);
    window.Run();

    _CrtDumpMemoryLeaks();
    return 0;
}