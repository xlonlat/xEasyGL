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
			ps.zFar = 100.0f;
			ps.fovy = 45.0f;
			ps.aspect = 1.0f * vs.w / vs.h;

			m_drawArgs = new xDrawArgs(vs, ps);

			camera == nullptr ? m_camera = new xCamera() : m_camera = camera;
			m_camera->Link(this);
		}

		xViewer::~xViewer(void)
		{
			delete m_camera;
			delete m_drawArgs;
		}

		void xViewer::Initialize()
		{
		}

		void xViewer::Render(double interval)
		{
			glClearDepth(1.0f);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);

			Begin2D(*m_drawArgs);
			{
				glColor3f(1.0f, 0.0f, 0.0f);
				glBegin(GL_LINES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(1000.0f, 1000.0f, 0.0f);
				glEnd();
			}

			Begin3D(*m_drawArgs);
			{

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

		const xDrawArgs& xViewer::DrawArgs() const
		{
			return *m_drawArgs;
		}

		void xViewer::Begin2D(const xDrawArgs& args)
		{
			const xViewportState& vs = args.vs();

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

		void xViewer::Begin3D(const xDrawArgs& args)
		{
			const xViewportState& vs = args.vs();
			const xProjState& ps = args.ps();

			// Set viewport .
			glViewport(0, 0, vs.w, vs.h);

			//glm::vec3 pos(0, 0, 1);
			//glm::vec3 tar(0, 0, 0);
			//glm::vec3 up(0, 1, 0);
			//glm::mat4 model = glm::lookAtRH(pos, tar, up);

			//static int print = 0;
			//if (!print++)
			//{
			//    xCommon::PrintMatrix4x4f(glm::value_ptr(model));
			//}
		}
	}
}
