#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xGlobal& xGlobal::m_instance = xGlobal::xGlobal();

		xGlobal::xGlobal()
		{
			// Initialize something...
			char* aaa = (char*)(new char[4]);
			char* bbb = (char*)malloc(4);
		}

		const xGlobal& xGlobal::Instance()
		{
			return m_instance;
		}

		const std::wstring& xGlobal::ResourcePath()
		{
			return L"";
		}

		xCamera::xCamera() : m_viewer(nullptr)
		{
		}

		xCamera::~xCamera()
		{
			m_viewer = nullptr;
		}

		void xCamera::Pan(int x, int y)
		{
		}

		void xCamera::Link(const xViewer* viewer)
		{
			m_viewer = viewer;
		}

		void xCamera::State(const xCameraState& state)
		{
			m_state = state;
		}
	}
}
