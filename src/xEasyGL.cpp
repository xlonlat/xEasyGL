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

			memset(m_resourcePath, 0, sizeof(m_resourcePath));
		}

		xGlobal::xGlobal(const xGlobal&)
		{
			memset(m_resourcePath, 0, sizeof(m_resourcePath));
		}

		const xGlobal& xGlobal::Instance()
		{
			return m_instance;
		}

		const wchar_t* xGlobal::ResourcePath()
		{
			return m_resourcePath;
		}

		xCamera::xCamera() : m_viewer(nullptr)
		{
		}

		xCamera::~xCamera()
		{
			m_viewer = nullptr;
		}

		void xCamera::Pan(int xoff, int yoff)
		{
			assert(m_viewer != nullptr);
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

			m_stateBak.pos += (1.0f * yoff * up * pixelSize);
			m_stateBak.pos -= (1.0f * xoff * right * pixelSize);
			m_stateBak.lookAt += (1.0f * yoff * up * pixelSize);
			m_stateBak.lookAt -= (1.0f * xoff * right * pixelSize);

			xProjState ps = args.ps();
			ps.zFar = fabs(m_stateBak.pos.z * 1.01f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			args.ps(ps);

			if (0)
			{
				float hgt = m_stateBak.pos.z;
				float fovy = args.ps().fovy;
				float zNear = args.ps().zNear;
				float height = args.vs().h * 1.f;
				float pixelSize = (tan(glm::radians(fovy / 2)) * (hgt - zNear)) / (height / 2);

				glm::vec3 off;
				off.x = xoff * pixelSize;
				off.y = yoff * pixelSize;
				off.z = 0;

				glm::vec3 forward = glm::normalize(m_stateBak.lookAt - m_stateBak.pos);

				m_stateBak.pos.x -= off.x;
				m_stateBak.pos.y += off.y;
				m_stateBak.lookAt.x -= off.x;
				m_stateBak.lookAt.y += off.y;
			}
		}

		void xCamera::Rotate(int xoff, int yoff)
		{
			assert(m_viewer != nullptr);
			if (xoff == 0 && yoff == 0) return;

			const xDrawArgs& args = m_viewer->DrawArgs();

			float width = args.vs().w * 1.f;
			float height = args.vs().h * 1.f;
			float length = std::min(width, height);

			glm::vec3 up = glm::normalize(m_stateBak.up);
			glm::vec3 forward = glm::normalize(m_stateBak.lookAt - m_stateBak.pos);
			glm::vec3 right = glm::normalize(glm::cross(forward, up));

			if (abs(xoff) >= abs(yoff))
			{
				float ang = glm::quarter_pi<float>() * xoff / length;

				glm::quat quat = glm::angleAxis(-ang, glm::vec3(0, 0, 1));

				forward = forward * quat;
				right = right * quat;
				up = glm::cross(right, forward);

				m_stateBak.up = up;
				m_stateBak.lookAt = m_state.pos + forward;
			}
			else
			{
				float ang = glm::quarter_pi<float>() * yoff / length;

				glm::quat quat = glm::angleAxis(-ang, right);

				up = up * quat;
				forward = forward * quat;
				right = glm::cross(forward, up);

				m_stateBak.up = up;
				m_stateBak.lookAt = m_state.pos + forward;
			}
		}

		void xCamera::Zoom(int cx, int cy, bool zoomin)
		{
			glm::vec3 forward = glm::normalize(m_stateBak.lookAt - m_stateBak.pos);

			xDrawArgs& args = m_viewer->DrawArgs();

			float dir = zoomin ? 1.f : -1.f;
			float len = m_stateBak.pos.z;

			m_stateBak.pos += (forward * dir * len * 0.1f);
			m_stateBak.lookAt = m_stateBak.pos + forward;

			xProjState ps = args.ps();
			ps.zFar = fabs(m_stateBak.pos.z * 1.01f);
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
