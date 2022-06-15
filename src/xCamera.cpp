#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xCamera::xCamera() : 
			m_Viewer(nullptr),
			m_Timespan(16), m_LastClock(0), m_FrameCounter(0)
		{
		}

		xCamera::~xCamera()
		{
		}

		void xCamera::Update()
		{
			memcpy(&m_State, &m_StatePre, sizeof(xCameraState));

			clock_t curr = clock();
			if (m_LastClock == 0) m_LastClock = curr;
			if (++m_FrameCounter > 10)
			{
				clock_t span = (curr - m_LastClock);
				m_Timespan = 1.0 * span / m_FrameCounter;
				m_FrameCounter = 0;
				m_LastClock = curr;
			}
		}

		glm::mat4 xCamera::ProjMat() const
		{
			const xProjState& ps = m_State.ps;
			const xViewportState& vs = m_State.vs;
			return glm::perspectiveFovRH(glm::radians(ps.fovy), (float)vs.w, (float)vs.h, ps.near, ps.far);
		}

		glm::mat4 xCamera::ViewMat() const
		{
			return glm::lookAtRH(m_State.eye, m_State.tar, m_State.up);
		}


		void xCamera::SetState(const xCameraState& state)
		{
			memcpy(&m_StatePre, &state, sizeof(xCameraState));
		}

		double xCamera::Timespan() const
		{
			return m_Timespan;
		}

		const xCameraState& xCamera::State() const
		{
			return m_State;
		}

		float xCamera::PixelSize(const xCameraState& state, float dis2cam) const
		{
			float fovy = state.ps.fovy;
			float near = state.ps.near;
			float height = 1.f * state.vs.h;
			return tan(glm::radians(fovy / 2)) * dis2cam / (height / 2);
		}


		xDefaultCamera::xDefaultCamera()
		{
			m_StatePre.eye = glm::vec3(0, 0, 100);
			m_StatePre.tar = glm::vec3(0, 0, 0);
			m_StatePre.up = glm::vec3(0, 1, 0);
			m_StatePre.ps.near = 0.01f;
			m_StatePre.ps.far = 100.1f;
			m_StatePre.ps.fovy = 45.f;

			Update();

			m_lastLDown.x = m_lastLDown.y = -1;
			m_lastRDown.x = m_lastRDown.y = -1;
		}

		xDefaultCamera::~xDefaultCamera()
		{

		}

		void xDefaultCamera::OnSize(int width, int height)
		{
			m_StatePre.vs.w = width;
			m_StatePre.vs.h = height;
			m_StatePre.ps.aspect = 1.f * width / height;

			Update();
		}

		void xDefaultCamera::OnLButtonUp(int cx, int cy)
		{
			m_lastLDown.x = m_lastLDown.y = -1;
		}

		void xDefaultCamera::OnRButtonUp(int cx, int cy)
		{
			m_lastRDown.x = m_lastRDown.y = -1;
		}

		void xDefaultCamera::OnMouseMove(int cx, int cy, int button)
		{
			xCameraState& state = m_StatePre; // or m_StatePre?

			if (button == 0)	// Left button.
			{
				if (m_lastLDown.x == -1 && m_lastLDown.y == -1)
				{
					m_lastLDown.x = cx;
					m_lastLDown.y = cy;
				}

				int xoff = cx - m_lastLDown.x;
				int yoff = cy - m_lastLDown.y;
				if (xoff != 0 || yoff != 0)
				{
					float fovy = state.ps.fovy;
					float near = state.ps.near;
					float height = 1.f * state.vs.h;
					float pixelSize = PixelSize(state, state.eye.z);

					glm::vec3 up = glm::normalize(state.up);
					glm::vec3 forward = glm::normalize(state.tar - state.eye);
					glm::vec3 right = glm::normalize(glm::cross(forward, up));
					up = glm::normalize(glm::cross(right, forward));

					m_StatePre.eye -= (1.0f * xoff * right * pixelSize);
					m_StatePre.eye += (1.0f * yoff * up * pixelSize);
					m_StatePre.tar -= (1.0f * xoff * right * pixelSize);
					m_StatePre.tar += (1.0f * yoff * up * pixelSize);

					state.ps.far = fabs(m_StatePre.eye.z * 2.f);
					state.ps.near = state.ps.far * 0.001f;
					if (state.ps.far < 1000) state.ps.far = 1000;
					if (state.ps.near < 1.0) state.ps.near = 0.001;
				}

				m_lastLDown.x = cx;
				m_lastLDown.y = cy;
			}
			else if (button == 1)	// Right button.
			{
				if (m_lastRDown.x == -1 && m_lastRDown.y == -1)
				{
					m_lastRDown.x = cx;
					m_lastRDown.y = cy;
				}

				int xoff = cx - m_lastRDown.x;
				int yoff = cy - m_lastRDown.y;
				if (xoff != 0 || yoff != 0)
				{
					float width = 1.f * state.vs.w;
					float height = 1.f * state.vs.h;
					float length = std::min(width, height);

					glm::vec3 up = glm::normalize(state.up);
					glm::vec3 forward = glm::normalize(state.tar - state.eye);
					glm::vec3 right = glm::normalize(glm::cross(forward, up));
					up = glm::normalize(glm::cross(right, forward));

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

						m_StatePre.up = up;
						m_StatePre.tar = m_StatePre.eye + forward;
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

						m_StatePre.up = up;
						m_StatePre.tar = m_StatePre.eye + forward;
					}

					state.ps.far = fabs(m_StatePre.eye.z * 2.f);
					state.ps.near = state.ps.far * 0.001f;
					if (state.ps.far < 1000) state.ps.far = 1000;
					if (state.ps.near < 1.0) state.ps.near = 0.001;
				}

				m_lastRDown.x = cx;
				m_lastRDown.y = cy;
			}
		}

		void xDefaultCamera::OnMouseWheel(int cx, int cy, int zoom)
		{
			xCameraState& state = m_StatePre; // or m_StatePre?

			glm::vec3 forward = glm::normalize(m_StatePre.tar - m_StatePre.eye);

			float len = state.eye.z;

			m_StatePre.eye += (forward * (float)zoom * len * 0.1f);
			m_StatePre.tar = m_StatePre.eye + forward;

			state.ps.far = fabs(m_StatePre.eye.z * 2.f);
			state.ps.near = state.ps.far * 0.001f;
			if (state.ps.far < 1000) state.ps.far = 1000;
			if (state.ps.near < 1.0) state.ps.near = 0.001;
		}
	}
}
