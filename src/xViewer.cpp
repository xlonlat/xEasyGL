#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xViewer::xViewer(xCamera* camera/* = nullptr */)
		{
			xViewportState vs;
			vs.x = 0;
			vs.y = 0;
			vs.w = 1280;
			vs.h = 720;

			xProjState ps;
			ps.zNear = 0.1f;
			ps.zFar = 1000.0f;
			ps.fovy = 45.0f;
			ps.aspect = 1.0f * vs.w / vs.h;

			xCameraState cam;
			cam.pos = glm::vec3(0.f, 0.f, 300.f);
			cam.lookAt = glm::vec3(0.f, 0.f, 0.f);
			cam.up = glm::vec3(0.f, 1.f, 0.f);

			m_drawArgs = new xDrawArgs(vs, ps);

			camera == nullptr ? m_camera = new xCamera() : m_camera = camera;
			m_camera->Link(this);
			m_camera->State(cam);

			m_lastLDown.x = m_lastLDown.y = -1;
			m_lastRDown.x = m_lastRDown.y = -1;
		}

		xViewer::~xViewer(void)
		{
			delete m_camera;
			delete m_drawArgs;
		}

		xDrawArgs& xViewer::DrawArgs() const
		{
			return *m_drawArgs;
		}

		void xViewer::Initialize()
		{
		}

		void xViewer::Render(double interval)
		{
			m_camera->Update();

			glClearDepth(1.0f);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);

			Begin2D();
			{
				glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(100.0f,   0.0f, 0.0f);
				glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(  0.0f, 100.0f, 0.0f);
				glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(200.0f, 100.0f, 0.0f);
				glEnd();
			}

			Begin3D();
			{
				glPushMatrix();
				glScaled(10.0, 10.0, 10.0);
				glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  1.0f, 0.0f);
				glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
				glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
				glEnd();
				glPopMatrix();

				glBegin(GL_LINES);
				for (int i=-100; i<=100; i+=2)
				{
					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(i * 1.0f,  100.0f, 0.0f);
					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(i * 1.0f, -100.0f, 0.0f);

					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f( 100.0f, i * 1.0f, 0.0f);
					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(-100.0f, i * 1.0f, 0.0f);
				}
				glEnd();

			}
		}

		void xViewer::Clear()
		{
		}

		void xViewer::Event(const xEvent& event)
		{
			switch (event.type)
			{
			case xEasyGL::Resize:
			{
				xViewportState vs = m_drawArgs->vs();
				vs.w = event.x;
				vs.h = event.y;
				m_drawArgs->vs(vs);

				xProjState ps = m_drawArgs->ps();
				ps.aspect = 1.0f * vs.w / vs.h;
				m_drawArgs->ps(ps);
			}break;
			default:
				break;
			}
		}

		void xViewer::OnLButtonUp(int cx, int cy)
		{
			m_lastLDown.x = m_lastLDown.y = -1;
		}

		void xViewer::OnRButtonUp(int cx, int cy)
		{
			m_lastRDown.x = m_lastRDown.y = -1;
		}

		void xViewer::OnMouseMove(int cx, int cy, int button)
		{
			if (button == 0)	// Left button.
			{
				if (m_lastLDown.x == -1 && m_lastLDown.y == -1)
				{
					m_lastLDown.x = cx;
					m_lastLDown.y = cy;
				}

				m_camera->Pan(cx - m_lastLDown.x, cy - m_lastLDown.y);

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

				m_camera->Rotate(cx - m_lastRDown.x, cy - m_lastRDown.y);

				m_lastRDown.x = cx;
				m_lastRDown.y = cy;
			}
		}

		void xViewer::OnMouseWheel(int cx, int cy, bool zoomin)
		{
			m_camera->Zoom(cx, cy, zoomin);
		}

		void xViewer::Begin2D()
		{
			const xViewportState& vs = m_drawArgs->vs();

			// Set viewport .
			glViewport(0, 0, vs.w, vs.h);

			// Set projection matrix.
			glm::mat4 proj = glm::ortho(0.0f, (float)vs.w, (float)vs.h, 0.0f);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(proj));

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		void xViewer::Begin3D()
		{
			const xViewportState& vs = m_drawArgs->vs();
			const xProjState& ps = m_drawArgs->ps();

			// Set viewport .
			glViewport(0, 0, vs.w, vs.h);

			// Set projection matrix.
			glm::mat4 proj = glm::perspectiveFovRH(glm::radians(ps.fovy), (float)vs.w, (float)vs.h, ps.zNear, ps.zFar);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(proj));

			const xCameraState& cam = m_camera->State();
			glm::mat4 model = glm::lookAtRH(cam.pos, cam.lookAt, cam.up);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(model));

			//static int print = 0;
			//if (!print++)
			//{
			//    xCommon::PrintMatrix4x4f(glm::value_ptr(model));
			//}
		}
	}
}
