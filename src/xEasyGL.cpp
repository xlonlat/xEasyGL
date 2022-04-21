#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xGlobal& xGlobal::m_instance = xGlobal::xGlobal();

		xGlobal::xGlobal()
		{
			// Initialize something...
		}

		const xGlobal& xGlobal::Instance()
		{
			return m_instance;
		}

		xCamera::xCamera() : m_viewer(nullptr)
		{
		}

		xCamera::~xCamera()
		{
			m_viewer = nullptr;
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
