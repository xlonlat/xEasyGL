#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xCamera::xCamera() : m_viewer(nullptr)
		{
		}

		xCamera::~xCamera()
		{
			m_viewer = nullptr;
		}

		void xCamera::Pan(const xMousePos& pos0, const xMousePos& pos1, int param)
		{
			assert(m_viewer != nullptr);

			int xoff = pos1.x - pos0.x;
			int yoff = pos1.y - pos0.y;

			if (xoff == 0 && yoff == 0) return;

			xDrawArgs& args = m_viewer->DrawArgs();

			float hgt = m_stateBak.pos.z;
			float fovy = args.ps().fovy;
			float zNear = args.ps().zNear;
			float height = args.vs().h * 1.f;
			float pixelSize = (tan(glm::radians(fovy / 2)) * (hgt - zNear)) / (height / 2);

			glm::vec3 up = glm::normalize(m_stateBak.up);
			glm::vec3 forward = glm::normalize(m_stateBak.lookAt - m_stateBak.pos);
			glm::vec3 right = glm::normalize(glm::cross(forward, up));

			m_stateBak.pos -= (1.0f * xoff * right * pixelSize);
			m_stateBak.pos += (1.0f * yoff * up * pixelSize);
			m_stateBak.lookAt -= (1.0f * xoff * right * pixelSize);
			m_stateBak.lookAt += (1.0f * yoff * up * pixelSize);

			xProjState ps = args.ps();
			ps.zFar = fabs(m_stateBak.pos.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			args.ps(ps);
		}

		void xCamera::Rotate(const xMousePos& pos0, const xMousePos& pos1, int param/*=0*/)
		{
			assert(m_viewer != nullptr);

			int xoff = pos1.x - pos0.x;
			int yoff = pos1.y - pos0.y;

			if (xoff == 0 && yoff == 0) return;

			xDrawArgs& args = m_viewer->DrawArgs();

			float width = args.vs().w * 1.f;
			float height = args.vs().h * 1.f;
			float length = std::min(width, height);

			glm::vec3 up = glm::normalize(m_stateBak.up);
			glm::vec3 forward = glm::normalize(m_stateBak.lookAt - m_stateBak.pos);
			glm::vec3 right = glm::normalize(glm::cross(forward, up));

			if (abs(xoff) >= abs(yoff))
			{
				float ang = glm::quarter_pi<float>() * xoff / length;

				glm::quat quat = glm::quat();
				quat = glm::rotate(quat, -ang, glm::vec3(0, 0, 1));

				// If the function 'glm::angleAxis' is used, vs will prompt 'warning C26812'.
				// glm::quat quat = glm::angleAxis(-ang, glm::vec3(0, 0, 1));

				forward = forward * quat;
				right = right * quat;
				up = glm::cross(right, forward);

				m_stateBak.up = up;
				m_stateBak.lookAt = m_state.pos + forward;
			}
			else
			{
				float ang = glm::quarter_pi<float>() * yoff / length;

				glm::quat quat = glm::quat();
				quat = glm::rotate(quat, -ang, right);

				// If the function 'glm::angleAxis' is used, vs will prompt 'warning C26812'.
				// glm::quat quat = glm::angleAxis(-ang, right);

				up = up * quat;
				forward = forward * quat;
				right = glm::cross(forward, up);

				m_stateBak.up = up;
				m_stateBak.lookAt = m_state.pos + forward;
			}

			xProjState ps = args.ps();
			ps.zFar = fabs(m_stateBak.pos.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			args.ps(ps);
		}

		void xCamera::Zoom(const xMousePos& pos, bool zoomin, int param)
		{
			assert(m_viewer != nullptr);

			glm::vec3 forward = glm::normalize(m_stateBak.lookAt - m_stateBak.pos);

			xDrawArgs& args = m_viewer->DrawArgs();

			float dir = zoomin ? 1.f : -1.f;
			float len = m_stateBak.pos.z;

			m_stateBak.pos += (forward * dir * len * 0.1f);
			m_stateBak.lookAt = m_stateBak.pos + forward;

			xProjState ps = args.ps();
			ps.zFar = fabs(m_stateBak.pos.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			args.ps(ps);
		}

		void xCamera::Update()
		{
			m_state = m_stateBak;
		}

		void xCamera::Link(const xViewer* viewer)
		{
			m_viewer = viewer;
		}

		void xCamera::State(const xCameraState& state)
		{
			m_stateBak = state;
		}
	}
}
