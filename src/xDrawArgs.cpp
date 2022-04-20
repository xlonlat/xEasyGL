#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xDrawArgs::xDrawArgs()
		{
			m_vs.width = 0.f;
			m_vs.heigth = 0.f;
			m_ps.zNear = 0.f;
			m_ps.zFar = 0.f;
			m_ps.fovy = 0.f;
			m_ps.aspect = 0.f;
		}

		xDrawArgs::xDrawArgs(const xViewportState& vs, const xProjectState& ps) : m_vs(vs), m_ps(ps)
		{

		}

		xDrawArgs::~xDrawArgs()
		{

		}

		const xViewportState* xDrawArgs::vs()
		{
			return &m_vs;
		}

		const xProjectState* xDrawArgs::ps()
		{
			return &m_ps;
		}
	}
}
