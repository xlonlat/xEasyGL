#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xDrawArgs::xDrawArgs()
		{
			m_vs.w = 0;
			m_vs.h = 0;
			m_ps.zNear = 0.f;
			m_ps.zFar = 0.f;
			m_ps.fovy = 0.f;
			m_ps.aspect = 0.f;
		}

		xDrawArgs::xDrawArgs(const xViewportState& vs, const xProjState& ps) : m_vs(vs), m_ps(ps)
		{

		}

		xDrawArgs::~xDrawArgs()
		{

		}

		const xViewportState& xDrawArgs::vs() const
		{
			return m_vs;
		}

		const xProjState& xDrawArgs::ps() const
		{
			return m_ps;
		}

		void xDrawArgs::vs(const xViewportState& vs)
		{
			m_vs = vs;
		}

		void xDrawArgs::ps(const xProjState& ps)
		{
			m_ps = ps;
		}
	}
}
