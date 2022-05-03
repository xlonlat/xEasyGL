#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xICamera::xICamera() : m_viewer(nullptr)
		{
		}

		xICamera::~xICamera() 
		{
		}

		void xICamera::Update()
		{
			m_state = m_statePre;
		}

		void xICamera::Link(const xViewer* viewer)
		{
			m_viewer = viewer;
		}

		void xICamera::State(const xCameraState& state)
		{
			m_statePre = state;
		}


		xFirstPersonCamera::xFirstPersonCamera()
		{
		}

		xFirstPersonCamera::~xFirstPersonCamera()
		{
			m_viewer = nullptr;
		}

		void xFirstPersonCamera::Pan(const xMousePos& pos0, const xMousePos& pos1, int param)
		{
			assert(m_viewer != nullptr);

			int xoff = pos1.x - pos0.x;
			int yoff = pos1.y - pos0.y;

			if (xoff == 0 && yoff == 0) return;

			xDrawArgs& args = m_viewer->DrawArgs();

			float hgt = m_statePre.pos.z;
			float fovy = args.ps().fovy;
			float zNear = args.ps().zNear;
			float height = args.vs().h * 1.f;
			float pixelSize = (tan(glm::radians(fovy / 2)) * (hgt - zNear)) / (height / 2);

			glm::vec3 up = glm::normalize(m_statePre.up);
			glm::vec3 forward = glm::normalize(m_statePre.lookAt - m_statePre.pos);
			glm::vec3 right = glm::normalize(glm::cross(forward, up));

			m_statePre.pos -= (1.0f * xoff * right * pixelSize);
			m_statePre.pos += (1.0f * yoff * up * pixelSize);
			m_statePre.lookAt -= (1.0f * xoff * right * pixelSize);
			m_statePre.lookAt += (1.0f * yoff * up * pixelSize);

			xProjState ps = args.ps();
			ps.zFar = fabs(m_statePre.pos.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			args.ps(ps);
		}

		void xFirstPersonCamera::Rotate(const xMousePos& pos0, const xMousePos& pos1, int param/*=0*/)
		{
			assert(m_viewer != nullptr);

			int xoff = pos1.x - pos0.x;
			int yoff = pos1.y - pos0.y;

			if (xoff == 0 && yoff == 0) return;

			xDrawArgs& args = m_viewer->DrawArgs();

			float width = args.vs().w * 1.f;
			float height = args.vs().h * 1.f;
			float length = std::min(width, height);

			glm::vec3 up = glm::normalize(m_statePre.up);
			glm::vec3 forward = glm::normalize(m_statePre.lookAt - m_statePre.pos);
			glm::vec3 right = glm::normalize(glm::cross(forward, up));

			//if (abs(xoff) >= abs(yoff))
			{
				float ang = glm::quarter_pi<float>() * xoff / length;

				glm::quat quat = glm::quat();
				quat = glm::rotate(quat, -ang, glm::vec3(0, 0, 1));

				// If the function 'glm::angleAxis' is used, vs will prompt 'warning C26812'.
				// glm::quat quat = glm::angleAxis(-ang, glm::vec3(0, 0, 1));

				forward = forward * quat;
				right = right * quat;
				up = glm::cross(right, forward);

				m_statePre.up = up;
				m_statePre.lookAt = m_state.pos + forward;
			}
			//else
			{
				float ang = glm::quarter_pi<float>() * yoff / length;

				glm::quat quat = glm::quat();
				quat = glm::rotate(quat, -ang, right);

				// If the function 'glm::angleAxis' is used, vs will prompt 'warning C26812'.
				// glm::quat quat = glm::angleAxis(-ang, right);

				up = up * quat;
				forward = forward * quat;
				right = glm::cross(forward, up);

				m_statePre.up = up;
				m_statePre.lookAt = m_state.pos + forward;
			}

			xProjState ps = args.ps();
			ps.zFar = fabs(m_statePre.pos.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			args.ps(ps);
		}

		void xFirstPersonCamera::Zoom(const xMousePos& pos, bool zoomin, int param)
		{
			assert(m_viewer != nullptr);

			glm::vec3 forward = glm::normalize(m_statePre.lookAt - m_statePre.pos);

			xDrawArgs& args = m_viewer->DrawArgs();

			float dir = zoomin ? 1.f : -1.f;
			float len = m_statePre.pos.z;

			m_statePre.pos += (forward * dir * len * 0.1f);
			m_statePre.lookAt = m_statePre.pos + forward;

			xProjState ps = args.ps();
			ps.zFar = fabs(m_statePre.pos.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			args.ps(ps);
		}
	}
}
