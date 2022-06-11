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

		void xCamera::Update()
		{
			m_state = m_statePre;
		}

		void xCamera::Link(const xViewer* viewer)
		{
			m_viewer = viewer;
		}

		void xCamera::SetState(const xCameraState& state)
		{
			m_statePre = state;
		}

		void xCamera::SetViewport(const xViewportState& vs)
		{
			m_statePre.vs = vs;
		}

		void xCamera::SetProjection(const xProjState& ps)
		{
			m_statePre.ps = ps;
		}


		xFirstPersonCamera::xFirstPersonCamera()
		{
		}

		xFirstPersonCamera::~xFirstPersonCamera()
		{
		}

		void xFirstPersonCamera::Pan(const glm::ivec2& posA, const glm::ivec2& posB, int param)
		{
			assert(m_viewer != nullptr);

			int xoff = posB.x - posA.x;
			int yoff = posB.y - posA.y;

			if (xoff == 0 && yoff == 0) return;

			const xCameraState& cam = m_viewer->Camera().State();
			xProjState ps = cam.ps;

			float hgt = m_statePre.eye.z;
			float fovy = ps.fovy;
			float zNear = ps.zNear;
			float height = cam.vs.h * 1.f;
			float pixelSize = (tan(glm::radians(fovy / 2)) * hgt) / (height / 2);

			glm::vec3 up = glm::normalize(m_statePre.up);
			glm::vec3 forward = glm::normalize(m_statePre.tar - m_statePre.eye);
			glm::vec3 right = glm::normalize(glm::cross(forward, up));

			m_statePre.eye -= (1.0f * xoff * right * pixelSize);
			m_statePre.eye += (1.0f * yoff * up * pixelSize);
			m_statePre.tar -= (1.0f * xoff * right * pixelSize);
			m_statePre.tar += (1.0f * yoff * up * pixelSize);

			ps.zFar = fabs(m_statePre.eye.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			m_viewer->Camera().SetProjection(ps);
		}

		void xFirstPersonCamera::Rotate(const glm::ivec2& posA, const glm::ivec2& posB, int param/*=0*/)
		{
			assert(m_viewer != nullptr);

			int xoff = posB.x - posA.x;
			int yoff = posB.y - posA.y;

			if (xoff == 0 && yoff == 0) return;

			const xCameraState& cam = m_viewer->Camera().State();
			xProjState ps = cam.ps;

			float width = cam.vs.w * 1.f;
			float height = cam.vs.h * 1.f;
			float length = std::min(width, height);

			glm::vec3 up = glm::normalize(m_statePre.up);
			glm::vec3 forward = glm::normalize(m_statePre.tar - m_statePre.eye);
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
				m_statePre.tar = m_state.eye + forward;
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
				m_statePre.tar = m_state.eye + forward;
			}

			ps.zFar = fabs(m_statePre.eye.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			m_viewer->Camera().SetProjection(ps);
		}

		void xFirstPersonCamera::Zoom(const glm::ivec2& pos, bool zoomin, int param)
		{
			assert(m_viewer != nullptr);

			glm::vec3 forward = glm::normalize(m_statePre.tar - m_statePre.eye);

			const xCameraState& cam = m_viewer->Camera().State();

			float dir = zoomin ? 1.f : -1.f;
			float len = m_statePre.eye.z;

			m_statePre.eye += (forward * dir * len * 0.1f);
			m_statePre.tar = m_statePre.eye + forward;

			xProjState ps = cam.ps;
			ps.zFar = fabs(m_statePre.eye.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			m_viewer->Camera().SetProjection(ps);
		}
	

		xMapCamera::xMapCamera()
		{

		}

		xMapCamera::~xMapCamera()
		{

		}

		void xMapCamera::Pan(const glm::ivec2& posA, const glm::ivec2& posB, int param)
		{
			assert(m_viewer != nullptr);

			int xoff = posB.x - posA.x;
			int yoff = posB.y - posA.y;

			if (xoff == 0 && yoff == 0) return;

			const xCameraState& cam = m_viewer->Camera().State();

			float hgt = m_statePre.eye.z;
			float fovy = cam.ps.fovy;
			float zNear = cam.ps.zNear;
			float height = cam.vs.h * 1.f;
			float pixelSize = (tan(glm::radians(fovy / 2)) * hgt) / (height / 2);

			glm::vec3 off = glm::vec3(-xoff, yoff, 0.0);

			m_statePre.eye += (1.0f * off * pixelSize);
			m_statePre.tar += (1.0f * off * pixelSize);

			xProjState ps = cam.ps;
			ps.zFar = fabs(m_statePre.eye.z * 2.f);
			if (ps.zFar < 1000) ps.zFar = 1000;
			ps.zNear = ps.zFar * 0.001f;
			m_viewer->Camera().SetProjection(ps);
		}

		void xMapCamera::Rotate(const glm::ivec2& posA, const glm::ivec2& posB, int param)
		{
		}

		void xMapCamera::Zoom(const glm::ivec2& pos, bool zoomin, int param)
		{

		}

	}
}
