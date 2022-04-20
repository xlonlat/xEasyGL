#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		static int flag = 0;

		xApplication::xApplication(const xViewer& viewer) : m_viewer(viewer)
		{
			if (!flag++)
			{
				glfwInit();
				glewInit();
			}
		}

		xApplication::~xApplication()
		{

		}

		void xApplication::Run()
		{

		}
	}
}
