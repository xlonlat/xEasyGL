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

		void xCamera::State(const xCameraState& state)
		{
			m_state = state;
		}

	}
}
